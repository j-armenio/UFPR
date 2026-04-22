# Documentação sci-kit knn
# https://scikit-learn.org/stable/modules/neighbors.html

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.neighbors import KNeighborsClassifier
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import classification_report, confusion_matrix
from sklearn.decomposition import PCA

# --------------------------------------------------
# Configuration
# --------------------------------------------------
FEATURES_PATHS = {
    1: {
        "name": "mobilenetv3",
        "train_csv": "features_csv/train_features_mobilenetv3.csv",
        "test_csv": "features_csv/test_features_mobilenetv3.csv",
    },
    2: {
        "name": "efficientnetv2",
        "train_csv": "features_csv/train_features_efficientnetv2.csv",
        "test_csv": "features_csv/test_features_efficientnetv2.csv",
    },
    3: {
        "name": "kimianet",
        "train_csv": "features_csv/train_features_kimianet.csv",
        "test_csv": "features_csv/test_features_kimianet.csv",
    }
}

# 1 - MobileNetV3 Small
# 2 - EfficientNetV2 Small
# 3 - KimiaNet
FEATURES_CHOICE = 3

TRAIN_CSV = FEATURES_PATHS[FEATURES_CHOICE]["train_csv"]
TEST_CSV = FEATURES_PATHS[FEATURES_CHOICE]["test_csv"]
N_NEIGHBORS = 3
METRIC = "euclidean"

PCA_REDUCTION = True
N_COMPONENTS = 0.9

# --------------------------------------------------
# Load CSV files
# --------------------------------------------------
print("Loading feature files...")

train_df = pd.read_csv(TRAIN_CSV)
test_df = pd.read_csv(TEST_CSV)

# --------------------------------------------------
# Split labels and features
# --------------------------------------------------
y_train = train_df["label"].values
X_train = train_df.drop(columns=["label"]).values

y_test = test_df["label"].values
X_test = test_df.drop(columns=["label"]).values

print("Train shape:", X_train.shape)
print("Test shape:", X_test.shape)

# --------------------------------------------------
# Feature normalization (critical for knn)
# --------------------------------------------------
scaler = StandardScaler()
X_train = scaler.fit_transform(X_train)
X_test = scaler.transform(X_test)

# --------------------------------------------------
# PCA (dimensionality reduction)
# --------------------------------------------------
if PCA_REDUCTION:
    pca = PCA(n_components=N_COMPONENTS)

    X_train = pca.fit_transform(X_train)
    X_test = pca.transform(X_test)

    print("After PCA:")
    print("Train shape:", X_train.shape)
    print("Test shape:", X_test.shape)

# --------------------------------------------------
# k-NN classifier
# --------------------------------------------------
knn = KNeighborsClassifier(
    n_neighbors=N_NEIGHBORS,
    metric=METRIC
)

print("Training k-NN...")
knn.fit(X_train, y_train)

# --------------------------------------------------
# Evaluation
# --------------------------------------------------
y_pred = knn.predict(X_test)

print("\nClassification Report (k-NN):")
print(classification_report(y_test, y_pred))

print("Confusion Matrix")
print(confusion_matrix(y_test, y_pred))
