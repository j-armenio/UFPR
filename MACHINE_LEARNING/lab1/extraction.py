import torch
import torch.nn as nn
from torchvision import datasets, transforms, models
from torch.utils.data import DataLoader, random_split
import numpy as np
import os
import csv
from tqdm import tqdm
import timm
from timm.data import resolve_data_config
from timm.data.transforms_factory import create_transform


# --------------------------------------------------
# Configuration
# --------------------------------------------------
DATA_DIR = "../datasets/Multi_Cancer_Lymphoma"
BATCH_SIZE = 32
TRAIN_SPLIT = 0.6
SEED = 42
OUTPUT_DIR = "features_csv"

# Model to extract the feature vector
# 1 - MobileNetV3 Small
# 2 - EfficientNetV2 Small
# 3 - DinoV2 Base
# 4 - KimiaNet
MODEL_CHOICE = 3

DEVICE = (
    "mps" if torch.backends.mps.is_available()
    else "cuda" if torch.cuda.is_available()
    else "cpu"
)

MODEL_PATHS = {
    1: {
        "name": "mobilenetv3",
        "train_csv": "train_features_mobilenetv3.csv",
        "test_csv": "test_features_mobilenetv3.csv",
    },
    2: {
        "name": "efficientnetv2",
        "train_csv": "train_features_efficientnetv2.csv",
        "test_csv": "test_features_efficientnetv2.csv",
    },
    3: {
        "name": "dinov2",
        "train_csv": "train_features_dinov2.csv",
        "test_csv": "test_features_dinov2.csv",
    },
    4: {
        "name": "kimianet",
        "train_csv": "train_features_kimianet.csv",
        "test_csv": "test_features_kimianet.csv",
    }
}

os.makedirs(OUTPUT_DIR, exist_ok=True)

# --------------------------------------------------
# Reproducibility
# --------------------------------------------------
torch.manual_seed(SEED)
np.random.seed(SEED)

# --------------------------------------------------
# Load Model + Normalization
# --------------------------------------------------

if MODEL_CHOICE == 1:
    weights = models.MobileNet_V3_Small_Weights.IMAGENET1K_V1
    transform = weights.transforms()
    model = models.mobilenet_v3_small(weights=weights)

    model.classifier = nn.Identity()

elif MODEL_CHOICE == 2:
    weights = models.EfficientNet_V2_S_Weights.DEFAULT
    transform = weights.transforms() 
    model = models.efficientnet_v2_s(weights=weights)

    model.classifier = nn.Identity()    

elif MODEL_CHOICE == 3:
    model = timm.create_model(
        "vit_base_patch14_dinov2.lvd142m",
        pretrained=True,
        num_classes=0 # remove classifier
    )

    config = resolve_data_config({}, model=model)
    transform = create_transform(**config)

elif MODEL_CHOICE == 4:
    model = models.densenet121(pretrained=False)

    state_dict = torch.load("../weights/KimiaNet.pth", map_location="cpu")
    model.load_state_dict(state_dict, strict=False)
    model.classifier = nn.Identity()

    transform = transforms.Compose([
        transforms.Resize(224),
        transforms.CenterCrop(224),
        transforms.ToTensor(),
        transforms.Normalize(
            mean=[0.485, 0.456, 0.406],
            std=[0.229, 0.224, 0.225]
        )
    ])

else:
    raise ValueError("Modelo inválido.")

model = model.to(DEVICE)
model.eval()

# --------------------------------------------------
# Dataset
# --------------------------------------------------
full_dataset = datasets.ImageFolder(DATA_DIR, transform=transform)
print("Classes:", full_dataset.classes)

# --------------------------------------------------
# Train / Test split (60 / 40)
# --------------------------------------------------
total_size = len(full_dataset)
train_size = int(TRAIN_SPLIT * total_size)
test_size = total_size - train_size

train_dataset, test_dataset = random_split(
    full_dataset,
    [train_size, test_size],
    generator=torch.Generator().manual_seed(SEED)
)

train_loader = DataLoader(train_dataset, batch_size=BATCH_SIZE, shuffle=False)
test_loader = DataLoader(test_dataset, batch_size=BATCH_SIZE, shuffle=False)

# --------------------------------------------------
# Feature Extraction
# --------------------------------------------------
def extract_features(loader):
    features, labels = [], []

    with torch.no_grad():
        for images, lbls in tqdm(loader, desc="Extracting features", unit="batch"):
            images = images.to(DEVICE)
            feats = model(images)

            if feats.dim() > 2:
                feats = feats.mean(dim=1)

            features.append(feats.cpu().numpy())
            labels.append(lbls.cpu().numpy())

    return np.vstack(features), np.hstack(labels)

# --------------------------------------------------
# CSV writer
# --------------------------------------------------
def save_csv(features, labels, filename):
    num_features = features.shape[1]

    header = ["label"] + [f"f{i}" for i in range(num_features)]

    with open(filename, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(header)

        for x, y in zip(features, labels):
            writer.writerow([y] + x.tolist())

# --------------------------------------------------
# Extract + Save
# --------------------------------------------------
X_train, y_train = extract_features(train_loader)
X_test, y_test = extract_features(test_loader)

print("Train features shape:", X_train.shape)
print("Test features shape:", X_test.shape)

paths = MODEL_PATHS.get(MODEL_CHOICE)
if paths is None:
    raise ValueError("Modelo inválido.")
OUTPUT_TRAIN_CSV = paths["train_csv"] 
OUTPUT_TEST_CSV = paths["test_csv"]

save_csv(
    X_train,
    y_train,
    os.path.join(OUTPUT_DIR, OUTPUT_TRAIN_CSV)
)

save_csv(
    X_test,
    y_test,
    os.path.join(OUTPUT_DIR, OUTPUT_TEST_CSV)
)

print("\nCSV files saved in", OUTPUT_DIR)