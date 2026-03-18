#!/usr/bin/env python3

import torch
import torch.nn as nn
from torchvision import datasets, transforms
from torchvision.models import mobilenet_v3_small, MobileNet_V3_Small_Weights
from torch.utils.data import DataLoader, random_split
import numpy as np
import random
from tqdm import tqdm
import csv
import os

# --------------------------------------------------
# Configuration
# --------------------------------------------------
DATA_DIR = "dataset"
BATCH_SIZE = 32
TRAIN_SPLIT = 0.6
SEED = 42
OUTPUT_DIR = "features_csv"

DEVICE = (
    "mps" if torch.backends.mps.is_available()
    else "cuda" if torch.cuda.is_available()
    else "cpu"
)

os.makedirs(OUTPUT_DIR, exist_ok=True)

# --------------------------------------------------
# Reproducibility
# --------------------------------------------------
torch.manual_seed(SEED)
np.random.seed(SEED)
random.seed(SEED)

# --------------------------------------------------
# Image normalization
# --------------------------------------------------
# padrão de normalização pytorch
transform = transforms.Compose([
    transforms.Resize((224, 224)), 
    transforms.ToTensor(),
    transforms.Normalize(
        mean=[0.485, 0.456, 0.406],
        std=[0.229, 0.224, 0.225]
    )
])

# --------------------------------------------------
# Dataset
# --------------------------------------------------
full_dataset = datasets.ImageFolder(DATA_DIR, transform=transform)
class_names = full_dataset.classes
print("Classes:", class_names)

# --------------------------------------------------
# Train / Test split (60 / 40) # treinamento / validação
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
# MobileNetV3 Small feature extractor (576-D)
# --------------------------------------------------
weights = MobileNet_V3_Small_Weights.IMAGENET1K_V1
model = mobilenet_v3_small(weights=weights)

# Remove classifier → output is 576-D
# vetor de caracteristica com 576 dimensoes
model.classifier = nn.Identity()

model = model.to(DEVICE)
model.eval()

for p in model.parameters():
    p.requires_grad = False

# --------------------------------------------------
# Feature extraction
# --------------------------------------------------
def extract_features(loader):
    features, labels = [], []

    with torch.no_grad():
        for images, lbls in tqdm(loader, desc="Extracting features", unit="batch"):
            images = images.to(DEVICE)
            feats = model(images)           # [B, 576]
            features.append(feats.cpu().numpy())
            labels.append(lbls.numpy())

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

save_csv(
    X_train,
    y_train,
    os.path.join(OUTPUT_DIR, "train_features.csv")
)

save_csv(
    X_test,
    y_test,
    os.path.join(OUTPUT_DIR, "test_features.csv")
)

print("\nCSV files saved in:", OUTPUT_DIR)

