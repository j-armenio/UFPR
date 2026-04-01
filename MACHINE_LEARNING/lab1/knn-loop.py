# Documentação sci-kit knn
# https://scikit-learn.org/stable/modules/neighbors.html

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.neighbors import KNeighborsClassifier
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import classification_report, confusion_matrix, accuracy_score
from sklearn.decomposition import PCA

# --------------------------------------------------
# Configuration
# --------------------------------------------------
FEATURES_PATH = {
    "MobileNetV3": {
        "train": "features_csv/train_features_mobilenetv3.csv",
        "test": "features_csv/test_features_mobilenetv3.csv",
        "color": "#e74c3c" 
    },
    "EfficientNetV2": {
        "train": "features_csv/train_features_efficientnetv2.csv",
        "test": "features_csv/test_features_efficientnetv2.csv",
        "color": "#2ecc71" 
    },
    "KimiaNet": {
        "train": "features_csv/train_features_kimianet.csv",
        "test": "features_csv/test_features_kimianet.csv",
        "color": "#3498db" 
    }
}

K_RANGE = range(1,31)

METRIC = "euclidean"
PCA_REDUCTION = False
N_COMPONENTS = 0.9

results = {}

sns.set_theme(style="whitegrid")
plt.figure(figsize=(14, 8))

# --------------------------------------------------
# Loop through the models
# --------------------------------------------------
for model_name, paths in FEATURES_PATH.items():
    print(f"\n--- Processing {model_name} ---")

    # Load
    train_df = pd.read_csv(paths["train"])
    test_df = pd.read_csv(paths["test"])

    # Split labels and features
    y_train = train_df["label"].values
    X_train = train_df.drop(columns=["label"]).values
    y_test = test_df["label"].values
    X_test = test_df.drop(columns=["label"]).values

    # Normalization
    scaler = StandardScaler()
    X_train = scaler.fit_transform(X_train)
    X_test = scaler.transform(X_test)

    # PCA
    if PCA_REDUCTION:
        pca = PCA(n_components=N_COMPONENTS)

        X_train = pca.fit_transform(X_train)
        X_test = pca.transform(X_test)

    # Loop over different k values
    model_accuracies = []
    for k in K_RANGE:
        knn = KNeighborsClassifier(
            n_neighbors=k,
            metric=METRIC
        )
        knn.fit(X_train, y_train)
            
        y_pred = knn.predict(X_test)
        acc = accuracy_score(y_test, y_pred)
        model_accuracies.append(acc)
    
    results[model_name] = model_accuracies

    # add curve to the graphic
    plt.plot(K_RANGE, model_accuracies, label=model_name,
            color=paths["color"], linewidth=2.5, marker='o', markersize=6)

# --------------------------------------------------
# Visualization
# --------------------------------------------------
plt.xlabel('Valor de K', fontsize=13)
plt.ylabel('Acurácia', fontsize=13)

plt.xticks(np.arange(1, 31, step=1))
plt.ylim(0.8, 1.0)

plt.legend(title="Modelos", title_fontsize='13', fontsize='11', frameon=True, shadow=True)

plt.tight_layout()
plt.show()