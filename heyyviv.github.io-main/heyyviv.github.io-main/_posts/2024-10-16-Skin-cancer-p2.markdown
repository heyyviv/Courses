---
layout: post
title: "Skin Cancer Part 2"
date: 2024-10-16 01:43:18 +0530
categories: kaggle
---

# LightGBM
```python
new_params = {
    "objective": "binary",
    "verbosity": -1,
    "boosting_type": "gbdt",
    "n_estimators": 200,
    'learning_rate': 0.05,    
    'lambda_l1': 0.0004681884533249742, 
    'lambda_l2': 8.765240856362274, 
    'num_leaves': 136, 
    'feature_fraction': 0.5392005444882538, 
    'bagging_fraction': 0.9577412548866563, 
    'bagging_freq': 6,
    'min_child_samples': 60,
    "device": "gpu"
}

scores = []
lgb_models = []
for fold in range(5):
    _df_train = new_train[new_train["fold"] != fold].reset_index(drop=True)
    _df_valid = new_train[new_train["fold"] == fold].reset_index(drop=True)
    model = lgb.LGBMRegressor(**new_params)
    #model = VotingClassifier([(f"lgb_{i}", lgb.LGBMClassifier(random_state=i, **new_params)) for i in range(3)], voting="soft")
    model.fit(_df_train[train_cols], _df_train["target"])
    preds = model.predict(_df_valid[train_cols])
    score = comp_score(_df_valid[["target"]], pd.DataFrame(preds, columns=["prediction"]), "")
    print(f"fold: {fold} - Partial AUC Score: {score:.5f}")
    scores.append(score)
    lgb_models.append(model)
```
objective: 'binary'
Specifies the learning task and the corresponding objective function. 'binary' indicates that the model is performing binary classification.
n_estimators: 500
The number of boosting iterations or trees to be built.
Controls the complexity and capacity of the model. More trees can potentially improve performance but may lead to overfitting.
learning_rate: 0.01
Determines the step size at each iteration while moving toward a minimum of the loss function.
bagging_freq: 1
In LightGBM, the parameter bagging_freq controls the frequency of bagging during the training process. Specifically, when bagging_freq is set to 1, it indicates that bagging will occur at every iteration of tree training. This means that a random subset of the training data, defined by the bagging_fraction, will be sampled for each tree built in the model.
pos_bagging_fraction: 0.75
The fraction of positive (class 1) samples to be used for each bagging iteration.
Helps in handling class imbalance by controlling the sampling rate of the positive class.
neg_bagging_fraction: 0.05
he fraction of negative (class 0) samples to be used for each bagging iteration.
Balances the dataset by undersampling the majority class (negative class) to mitigate class imbalance.
feature_fraction: 0.8
The fraction of features to consider when building each tree.
lambda_l1: 0.8
L1 regularization term on weights. It adds a penalty equal to the absolute value of the magnitude of coefficients.
lambda_l2: 0.8
L2 regularization term on weights. It adds a penalty equal to the square of the magnitude of coefficients.

# CatBoost
```python
cb_params = {
    'objective': 'Logloss',
    # "random_state": 42,
    # "colsample_bylevel": 0.3, # 0.01, 0.1
    "iterations": 400,
    "learning_rate": 0.05,
    "cat_features": cat_cols,
    "max_depth": 8,
    "l2_leaf_reg": 5,
    "task_type": "GPU",
    # "scale_pos_weight": 2,
    "verbose": 0,
}

cb_scores = []
cb_models = []
for fold in range(5):
    _df_train = new_train[new_train["fold"] != fold].reset_index(drop=True)
    _df_valid = new_train[new_train["fold"] == fold].reset_index(drop=True)
    # model = cb.CatBoostClassifier(**cb_params)
    model = VotingClassifier([(f"cb_{i}", cb.CatBoostClassifier(random_state=i, **cb_params)) for i in range(3)], voting="soft")
    # eval_set=(_df_valid[train_cols], _df_valid["target"]), early_stopping_rounds=50
    model.fit(_df_train[train_cols], _df_train["target"])
    preds = model.predict_proba(_df_valid[train_cols])[:, 1]
    score = comp_score(_df_valid[["target"]], pd.DataFrame(preds, columns=["prediction"]), "")
    print(f"fold: {fold} - Partial AUC Score: {score:.5f}")
    cb_scores.append(score)
    cb_models.append(model)
```
objective: 'Logloss'
Defines the loss function that CatBoost will optimize during training.
iterations: 400
Specifies the number of boosting iterations (i.e., the number of trees) the model will build.
learning_rate: 0.05
Determines the step size at each boosting iteration while moving toward a minimum of the loss function.
cat_features: cat_cols
Specifies the categorical features in the dataset that CatBoost should handle internally.
l2_leaf_reg: 5
L2 regularization coefficient applied to leaf scores to prevent overfitting.

# Gradiant boosted trees
The learning rate (often denoted as eta in some libraries) is a hyperparameter that controls the step size at each iteration while moving toward a minimum of the loss function. In the context of gradient-boosted trees, it determines how much each new tree influences the overall model.
New Model=Old Model+Learning Rate×New Tree Predictions
L1 regularization, also known as Lasso regularization, adds a penalty equal to the absolute value of the magnitude of coefficients to the loss function. In the context of gradient-boosted trees, it penalizes the complexity of the model by encouraging sparsity in the leaf scores (the output values at the leaves of the trees).
L_{new} = L + \lambda \sum_{j=1}^{T} |w_{j}|
λ: Regularization parameter controlling the strength of the penalty.
w: Leaf scores (predictions) of the trees.
L2 Regularization
L2 regularization, also known as Ridge regularization, adds a penalty equal to the square of the magnitude of coefficients to the loss function. In gradient-boosted trees, it penalizes the complexity by discouraging large leaf scores, promoting smoother and more generalizable models.
The original loss function 𝐿 is augmented with an L2 penalty term.
Certainly! Below is the expression formatted in Markdown with appropriate mathematical notation:

$$
L_{\text{new}} = L + \lambda \sum_{j=1}^{T} w_j^2
$$

- $$ L_{\text{new}} $$: The updated value of $$ L $$.
- $$ L $$: The original value.
- $$ \lambda $$: A parameter that scales the sum.
- $$ T $$: The upper limit of the summation.
- $$ w_j $$: A variable indexed by $$ j $$.

# EfficientNet

We taking ratio of positive to negative 1:20
```python
df_positive = df[df["target"] == 1].reset_index(drop=True)
df_negative = df[df["target"] == 0].reset_index(drop=True)

df = pd.concat([df_positive, df_negative.iloc[:df_positive.shape[0]*20, :]])
```
# StratifiedGroupKFold
StratifiedGroupKFold is a cross-validation splitter provided by scikit-learn that combines the functionalities of both stratification and grouping. It ensures that each fold of the cross-validation process maintains the same class distribution (stratification) and that the samples from the same group are not split across different folds (grouping). This is particularly useful in scenarios where:
Class Imbalance: The dataset has imbalanced classes, and you want each fold to reflect the overall class distribution.
Grouped Data: Data points are naturally grouped (e.g., multiple samples from the same subject, multiple transactions from the same customer), and you want to ensure that all samples from a group are assigned to the same fold to prevent data leakage.
- Class Imbalance: The dataset has imbalanced classes, and you want each fold to reflect the overall class distribution.
- Grouped Data: Data points are naturally grouped (e.g., multiple samples from the same subject, multiple transactions from the same customer), and you want to ensure that all samples from a group are assigned to the same fold to prevent data leakage.

### Stratification:
- Ensures that each fold has approximately the same percentage of samples of each target class as the complete dataset.
- This is crucial for classification tasks where maintaining the class distribution across folds leads to more reliable and unbiased evaluation metrics.
- For example, if you have a binary classification problem with 30% positives (target=1) and 70% negatives (target=0), each fold should aim to preserve this 30:70 ratio.
### Grouping:
- Ensures that all samples within a single group are entirely in one fold. This prevents scenarios where the model is trained on data from a group and tested on another sample from the same group, which could lead to overly optimistic performance estimates.

Combining Stratification and Grouping
Balancing class distribution while ensuring group integrity can be conflicting objectives, especially when groups vary in size and class composition.

# Dataset
In Dataset with probability of 0.5 we are returning positive or negative row
```python
if random.random() >= 0.5:
            df = self.df_positive
            file_names = self.file_names_positive
            targets = self.targets_positive
        else:
            df = self.df_negative
            file_names = self.file_names_negative
            targets = self.targets_negative
        index = index % df.shape[0]
        
        img_path = file_names[index]
        img = cv2.imread(img_path)
        img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        target = targets[index]
        
        if self.transforms:
            img = self.transforms(image=img)["image"]
            
        return {
            'image': img,
            'target': target
        }
```

# Augmentation
```python
data_transforms = {
    "train": A.Compose([
        A.Resize(CONFIG['img_size'], CONFIG['img_size']),
        A.RandomRotate90(p=0.5),
        A.Flip(p=0.5),
        A.Downscale(p=0.25),
        A.ShiftScaleRotate(shift_limit=0.1, 
                           scale_limit=0.15, 
                           rotate_limit=60, 
                           p=0.5),
        A.HueSaturationValue(
                hue_shift_limit=0.2, 
                sat_shift_limit=0.2, 
                val_shift_limit=0.2, 
                p=0.5
            ),
        A.RandomBrightnessContrast(
                brightness_limit=(-0.1,0.1), 
                contrast_limit=(-0.1, 0.1), 
                p=0.5
            ),
        A.Normalize(
                mean=[0.485, 0.456, 0.406], 
                std=[0.229, 0.224, 0.225], 
                max_pixel_value=255.0, 
                p=1.0
            ),
        ToTensorV2()], p=1.),
    
    "valid": A.Compose([
        A.Resize(CONFIG['img_size'], CONFIG['img_size']),
        A.Normalize(
                mean=[0.485, 0.456, 0.406], 
                std=[0.229, 0.224, 0.225], 
                max_pixel_value=255.0, 
                p=1.0
            ),
        ToTensorV2()], p=1.)
}
```
# GEM Pooling
```python
class GeM(nn.Module):
    def __init__(self, p=3, eps=1e-6):
        super(GeM, self).__init__()
        self.p = nn.Parameter(torch.ones(1)*p)
        self.eps = eps

    def forward(self, x):
        return self.gem(x, p=self.p, eps=self.eps)
        
    def gem(self, x, p=3, eps=1e-6):
        return F.avg_pool2d(x.clamp(min=eps).pow(p), (x.size(-2), x.size(-1))).pow(1./p)
        
    def __repr__(self):
        return self.__class__.__name__ + \
                '(' + 'p=' + '{:.4f}'.format(self.p.data.tolist()[0]) + \
                ', ' + 'eps=' + str(self.eps) + ')'
```
# Model
```python
class ISICModel(nn.Module):
    def __init__(self, model_name, num_classes=1, pretrained=True, checkpoint_path=None):
        super(ISICModel, self).__init__()
        self.model = timm.create_model(model_name, pretrained=pretrained, checkpoint_path=checkpoint_path)

        in_features = self.model.classifier.in_features
        self.model.classifier = nn.Identity()
        self.model.global_pool = nn.Identity()
        self.pooling = GeM()
        self.linear = nn.Linear(in_features, num_classes)
        self.sigmoid = nn.Sigmoid()

    def forward(self, images):
        features = self.model(images)
        pooled_features = self.pooling(features).flatten(1)
        output = self.sigmoid(self.linear(pooled_features))
        return output

```

# Loss
```python
def criterion(outputs, targets):
    return nn.BCELoss()(outputs, targets)
```
# Optimizer
```python
optimizer = optim.Adam(model.parameters(), lr=CONFIG['learning_rate'], 
                       weight_decay=CONFIG['weight_decay'])
```
# Scheduler
Scheduler used is CosineAnnealingLR
```python
  scheduler = lr_scheduler.CosineAnnealingLR(optimizer,T_max=CONFIG['T_max'], 
                                                   eta_min=CONFIG['min_lr'])
```
T_max = 500
min_lr = 1e-6
Cosine Annealing is a strategy that gradually decreases the learning rate following a cosine curve without restarting. The learning rate starts at an initial value, decreases to a minimum value, and follows a smooth, cosine-shaped curve. This approach helps the optimizer to explore the loss landscape more effectively and can escape local minima, leading to better convergence.
Certainly! Here's the formula for **`CosineAnnealingLR`** written in Markdown using LaTeX syntax:

Here's the content rewritten in a Markdown-friendly way:

# Cosine Annealing Learning Rate Schedule

## Formula

The learning rate at epoch t is given by:

```
η_t = η_min + 0.5 * (η_initial - η_min) * (1 + cos(T_cur / T_max * π))
```

## Explanation of the Formula Components

- **η_t**: Learning rate at epoch t. This is the adjusted learning rate after applying the cosine annealing schedule.
- **η_initial**: Initial learning rate. The starting learning rate before any adjustments.
- **η_min**: Minimum learning rate. The lowest learning rate that the scheduler will anneal to.
- **T_cur**: Current epoch number since the last restart (if using restarts).
- **T_max**: Maximum number of epochs for one complete cosine cycle.

## Detailed Breakdown

1. **Cosine Decay Component**: `cos(T_cur / T_max * π)`
   - **Purpose**: Modulates the learning rate following a cosine curve.
   - **Behavior**:
     - At T_cur = 0, cos(0) = 1, so the learning rate starts at η_initial.
     - At T_cur = T_max, cos(π) = -1, reducing the learning rate to η_min.
     - The learning rate decreases smoothly from η_initial to η_min over T_max epochs.

2. **Scaling and Shifting**: `0.5 * (η_initial - η_min) * (1 + cos(T_cur / T_max * π))`
   - **Scaling**: The difference (η_initial - η_min) determines the range over which the learning rate will oscillate.
   - **Shifting**: Adding 1 inside the cosine term ensures that the cosine function shifts from [-1, 1] to [0, 2], allowing the learning rate to oscillate between η_min and η_initial.

3. **Final Adjustment**: `η_t = η_min + Scaled Cosine Term`
   - **Purpose**: Ensures that the learning rate never falls below η_min, providing a lower bound.

## Warm Restarts and Model Divergence

Warm restarts usually cause the model to diverge intentionally. This controlled divergence allows the model to work around local minima in the task's cost surface, potentially finding a better global minimum. It's analogous to finding a valley, climbing a nearby hill, and discovering an even deeper valley in another region.

### Visual Summary

Imagine two scenarios:

1. A learner that converges slowly along a low-gradient path.
2. A learner that uses warm restarts to fall into and climb out of a sequence of local minima.

Both learners may converge to the same global minimum, but the second approach often finds it faster due to following a path with a much higher overall gradient.

![Graph](/assets/isic_4.jpg)

Warm restarts usually actually cause the model to diverge. This is done on purpose. It turns out that adding some controlled divergence allows the model to work around local minima in the task's cost surface, allowing it to find an even better global minima instead. This is akin to finding a valley, then climbing a nearby hill, and discovering an even deeper valley one region over. Here's a visual summary:

![Graph](/assets/isic_5.jpg)

Both of these learners converge to the same global minima. However, on the left, the learner trundles slowly along a low-gradient path. On the right, the learner falls into a sequence of local minima (valleys), then uses warm restarts to climb over them (hills). In the process it finds the same global minima faster, because the path it follows has a much higher gradient overall.

