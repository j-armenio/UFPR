# Documentação sci-kit knn
# https://scikit-learn.org/stable/modules/neighbors.html

import numpy as np
import pandas as pd
from sklearn.neighbors import KNeighborsClassifier
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import classification_report, confusion_matrix
from sklearn.decomposition import PCA

# --------------------------------------------------
# Configuration
# --------------------------------------------------
TRAIN_CSV = "features_csv/train_features_dinov2.csv"
TEST_CSV = "features_csv/test_features_dinov2.csv"
N_NEIGHBORS = 7
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