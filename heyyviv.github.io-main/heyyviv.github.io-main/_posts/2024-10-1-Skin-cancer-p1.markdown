---
layout: post
title: "Skin Cancer Part 1"
date: 2024-10-01 01:43:18 +0530
categories: kaggle
---

# ISIC 2024 - Skin Cancer Detection with 3D-TBP 
Skin cancer can be deadly if not caught early, but many populations lack specialized dermatologic care. Over the past several years, dermoscopy-based AI algorithms have been shown to benefit clinicians in diagnosing melanoma, basal cell, and squamous cell carcinoma. However, determining which individuals should see a clinician in the first place has great potential impact. Triaging applications have a significant potential to benefit underserved populations and improve early skin cancer detection, the key factor in long-term patient outcomes.

In this project, I aimed to develop a binary classification algorithm to identify skin cancer from single-lesion crops of 3D total body photos (TBP). Skin cancer, if not detected early, can be life-threatening, especially in underserved populations lacking access to specialized dermatologic care. Given that telehealth submissions often involve lower-quality images, similar to smartphone photos, my goal was to create an algorithm that works in these non-clinical settings.

By building this project, I sought to extend the benefits of AI-based skin cancer detection to broader populations, enhancing early diagnosis and triage in resource-constrained environments.

In this competition, we  develop image-based algorithms to identify histologically confirmed skin cancer cases with single-lesion crops from 3D total body photos (TBP). The image quality resembles close-up smartphone photos, which are regularly submitted for telehealth purposes. Our binary classification algorithm could be used in settings without access to specialized care and improve triage for early skin cancer detection.

Dermatoscope images reveal morphologic features not visible to the naked eye, but these images are typically only captured in dermatology clinics. Algorithms that benefit people in primary care or non-clinical settings must be adept to evaluating lower quality images. This competition leverages 3D TBP to present a novel dataset of every single lesion from thousands of patients across three continents with images resembling cell phone photos.

# Evaluation 

![table](/assets/isic_2.jpg)

# Confusion Matrix Example

A confusion matrix is a useful tool for evaluating the performance of a classification model.

## Confusion Matrix

|                | Predicted Positive | Predicted Negative |
|----------------|---------------------|---------------------|
| **Actual Positive** | True Positive (TP)  | False Negative (FN)  |
| **Actual Negative** | False Positive (FP) | True Negative (TN)   |

### Explanation of Terms

- **True Positive (TP)**: The number of positive instances correctly predicted as positive.
- **False Negative (FN)**: The number of positive instances incorrectly predicted as negative.
- **False Positive (FP)**: The number of negative instances incorrectly predicted as positive.
- **True Negative (TN)**: The number of negative instances correctly predicted as negative.

### Example Values

To give you a clearer picture, here's an example with hypothetical values:

|                | Predicted Positive | Predicted Negative |
|----------------|---------------------|---------------------|
| **Actual Positive** | 50                  | 10                  |
| **Actual Negative** | 5                   | 100                 |

### Performance Metrics

From this confusion matrix, you can derive several performance metrics:

- **Accuracy**: $$(TP + TN) / (TP + TN + FP + FN)$$
- **Precision**: $$TP / (TP + FP)$$
- **Recall (Sensitivity)**: $$TP / (TP + FN)$$
- **F1 Score**: $$2 \times \frac{Precision \times Recall}{Precision + Recall}$$

Accuracy is a widely used metric in machine learning, but it can be misleading in many contexts:
- Imbalance in Data (Class Imbalance Problem)
Description: Accuracy is unreliable when dealing with imbalanced datasets, where one class is significantly more frequent than others.
Example: Consider a dataset with 95% of class A and 5% of class B. A model that predicts class A for every instance would still achieve an accuracy of 95%, even though it's failing to detect class B. This would give a false sense of model performance.
Better Metrics: Precision, recall, F1-score, or area under the ROC curve (AUC-ROC) provide more insight into the performance of models dealing with imbalanced data.
- Inability to Capture the Severity of Errors
Description: Accuracy does not distinguish between different types of errors or their consequences. It treats all incorrect predictions the same, regardless of the severity.
Example: In medical diagnosis, predicting that a patient is healthy when they have a life-threatening disease (false negative) is far more critical than wrongly predicting a healthy person as ill (false positive). Accuracy would not capture this difference.
Better Metrics: In such cases, you can use weighted accuracy or specific metrics like sensitivity (recall) and specificity to reflect the consequences of false negatives and false positives.
- Lack of Sensitivity to Data Distribution
Description: Accuracy does not account for the distribution of classes or categories. It assumes that all classes are equally important or that misclassification has the same impact across the dataset.
Example: If you're predicting fraud in transactions, where 1% of the data represents fraudulent transactions and 99% represents legitimate transactions, a high accuracy may just reflect the model's ability to predict the majority class (legitimate transactions), but not fraud.
Better Metrics: You can use precision-recall curves or confusion matrices to get more granular insights into how the model performs on each class, especially the minority class.


# TPR and FPR

## True Positive Rate (TPR)
- **Definition**: Proportion of actual positives correctly identified.
- **Formula**: 
  $$
  \text{TPR} = \frac{TP}{TP + FN}
  $$

## False Positive Rate (FPR)
- **Definition**: Proportion of actual negatives incorrectly identified as positives.
- **Formula**: 
  $$
  \text{FPR} = \frac{FP}{FP + TN}
  $$

### Summary Table

| Metric | Formula |
|--------|---------|
| TPR    | $$\frac{TP}{TP + FN}$$ |
| FPR    | $$\frac{FP}{FP + TN}$$ |



![table](/assets/isic_1.jpg)

## Precision-Recall Curve

### Introduction

The **Precision-Recall (PR) curve** is an evaluation metric for binary classification problems, particularly useful when dealing with **imbalanced datasets**. It provides insights into the trade-off between two key performance metrics: **precision** and **recall**.

### Key Metrics

1. **Precision (Positive Predictive Value)**: The fraction of relevant instances among the retrieved instances.
   
   \[
   \text{Precision} = \frac{\text{True Positives (TP)}}{\text{True Positives (TP)} + \text{False Positives (FP)}}
   \]
   
   - High precision means fewer false positives.

2. **Recall (Sensitivity or True Positive Rate)**: The fraction of relevant instances that were retrieved.
   
   \[
   \text{Recall} = \frac{\text{True Positives (TP)}}{\text{True Positives (TP)} + \text{False Negatives (FN)}}
   \]
   
   - High recall means fewer false negatives.

### Trade-off Between Precision and Recall

- **Precision and recall** are inversely related. Increasing recall often reduces precision and vice versa.
- **Threshold tuning** plays a key role in this balance:
  - A **higher threshold** increases precision but decreases recall.
  - A **lower threshold** increases recall but decreases precision.

### PR Curve Plot

The **Precision-Recall curve** is plotted with:

- **X-axis**: Recall (ranging from 0 to 1)
- **Y-axis**: Precision (ranging from 0 to 1)

### Example PR Curve

Here’s a Python example to generate a Precision-Recall curve:

```python
import numpy as np
from sklearn.metrics import precision_recall_curve
import matplotlib.pyplot as plt

# Simulated true labels and model predicted probabilities
y_true = np.array([0, 0, 1, 1, 1, 0, 0, 1, 1, 0])
y_scores = np.array([0.1, 0.4, 0.35, 0.8, 0.7, 0.5, 0.3, 0.9, 0.65, 0.2])

# Calculate precision, recall, and thresholds
precision, recall, thresholds = precision_recall_curve(y_true, y_scores)

# Plot the precision-recall curve
plt.figure(figsize=(8, 6))
plt.plot(recall, precision, marker='.', label='Precision-Recall Curve')

# Labeling the axes
plt.xlabel('Recall')
plt.ylabel('Precision')
plt.title('Precision-Recall Curve')

# Show the plot
plt.legend()
plt.show()
```

### Interpretation

- **Upper-right region**: Represents models with both high precision and high recall, indicating good performance.
- **Lower-left region**: Represents poor performance, with low precision and recall.
- A high precision-recall curve means that the model maintains good precision as recall increases.

### Applications of PR Curve

- **Imbalanced Data**: PR curves are particularly valuable for datasets with skewed class distributions (e.g., fraud detection, medical diagnosis).
- **Threshold Selection**: By analyzing the PR curve, you can select the best classification threshold that balances precision and recall.

### Area Under the Precision-Recall Curve (AUC-PR)

The **AUC-PR** measures the area under the PR curve, indicating overall model performance. A higher area signifies better performance, particularly focusing on the positive class.

---

## ROC Curve and AUC

### ROC Curve Overview

The **Receiver Operating Characteristic (ROC) curve** is used to evaluate the performance of binary classification models by plotting the trade-off between **True Positive Rate (TPR)** and **False Positive Rate (FPR)** across different thresholds.

- **True Positive Rate (TPR)** (also known as **Recall**):
  
  \[
  \text{TPR} = \frac{\text{True Positives (TP)}}{\text{True Positives (TP)} + \text{False Negatives (FN)}}
  \]

- **False Positive Rate (FPR)**:
  
  \[
  \text{FPR} = \frac{\text{False Positives (FP)}}{\text{False Positives (FP)} + \text{True Negatives (TN)}}
  \]

The ROC curve is generated by calculating TPR and FPR at various thresholds. A model that performs well will have a curve that hugs the upper left corner.

### Example ROC Curve

Here’s a Python example to generate an ROC curve:

```python
import numpy as np
from sklearn.metrics import roc_curve
import matplotlib.pyplot as plt

# Simulated true labels and model predicted probabilities
y_true = np.array([0, 0, 1, 1, 1, 0, 0, 1, 1, 0])
y_scores = np.array([0.1, 0.4, 0.35, 0.8, 0.7, 0.5, 0.3, 0.9, 0.65, 0.2])

# Compute ROC curve
fpr, tpr, thresholds = roc_curve(y_true, y_scores)

# Plot ROC curve
plt.figure(figsize=(8, 6))
plt.plot(fpr, tpr, marker='.', label='ROC Curve')

# Labeling the axes
plt.xlabel('False Positive Rate')
plt.ylabel('True Positive Rate')
plt.title('ROC Curve')

# Show the plot
plt.legend()
plt.show()
```

### AUC (Area Under the Curve)

- The **AUC** quantifies the overall performance of the model by measuring the area under the ROC curve.
  
  - **AUC = 1**: Perfect model.
  - **AUC = 0.5**: No better than random guessing.

### Applications

- **Model Comparison**: AUC helps compare different models.
- **Threshold Selection**: The ROC curve aids in selecting the optimal threshold based on the trade-off between TPR and FPR.

---

The ROC curve can only be plotted for models that output predicted probabilities, such as logistic regression and random forest. Other models such as Naive Bayes do not, and therefore ROC curves cannot be plotted for these models.
In order to compare the performance of different classifiers, the standard practice is to plot the ROC curve and then compare the AUC values. The model with the higher AUC value is considered to be more performant.

## Partial ROC Curve

### Introduction

The **Partial ROC Curve (pROC)** focuses on a specific portion of the ROC curve, typically the range where **False Positive Rate (FPR)** is low. This is especially useful in domains where minimizing false positives is crucial, such as **medical diagnosis** or **fraud detection**.

### Example Partial ROC Curve

Here’s a Python example to generate a Partial ROC Curve:

```python
import numpy as np
from sklearn.metrics import roc_curve, auc
import matplotlib.pyplot as plt

# Simulated true labels and model predicted probabilities
y_true = np.array([0, 0, 1, 1, 1, 0, 0, 1, 1, 0])
y_scores = np.array([0.1, 0.4, 0.35, 0.8, 0.7, 0.5, 0.3, 0.9, 0.65, 0.2])

# Compute ROC curve
fpr, tpr, thresholds = roc_curve(y_true, y_scores)

# Define range of FPR for the partial ROC curve (e.g., FPR < 0.1)
max_fpr = 0.1
partial_fpr = fpr[fpr <= max_fpr]
partial_tpr = tpr[:len(partial_fpr)]

# Calculate partial AUC (optional)
partial_auc = auc(partial_fpr, partial_tpr)

# Plot the partial ROC curve
plt.figure(figsize=(8, 6))
plt.plot(partial_fpr, partial_tpr, marker='.', label=f'Partial ROC (AUC = {partial_auc:.2f})')

# Labeling the axes
plt.xlabel('False Positive Rate')
plt.ylabel('True Positive Rate')
plt.title('Partial ROC Curve')

# Show the plot
plt.legend()
plt.show()
```

### Applications of Partial ROC

- **Medical Diagnosis**: Avoiding false positives is critical, so performance at low FPR is emphasized.
- **Fraud Detection**: Limiting false positives prevents unnecessary investigations.
- **Anomaly Detection**: Ensures minimal false alarms by focusing on low FPR ranges.

### Partial AUC (pAUC)

The **Partial AUC (pAUC)** measures the area under the ROC curve within a specified FPR range, providing a more relevant metric in scenarios where low FPR is crucial.

### Advantages of pROC

- **Focus on Relevant Performance**: Highlights performance in regions of interest, such as low FPR.
- **Better Decision-Making**: Allows stakeholders to make informed choices when high false positives are unacceptable.

### Limitations of pROC

- **Arbitrary FPR Threshold**: The selection of an FPR threshold can be subjective and domain-dependent.
- **Loss of Full Picture**: Focusing on a partial region may hide useful information about model performance at higher FPR values.

---

### Summary

- **Precision-Recall Curve** is ideal for imbalanced datasets, showing the trade-off between precision and recall.
- **ROC Curve** provides a comprehensive view of a model’s classification performance.
- **Partial ROC Curve** zooms in on regions of interest (low FPR), making it crucial in domains where controlling false positives is important.

# Primary Scoring Metric
Submissions are evaluated on partial area under the ROC curve (pAUC) above 80% true positive rate (TPR) for binary classification of malignant examples. (See the implementation in the notebook ISIC pAUC-aboveTPR.)

The receiver operating characteristic (ROC) curve illustrates the diagnostic ability of a given binary classifier system as its discrimination threshold is varied. However, there are regions in the ROC space where the values of TPR are unacceptable in clinical practice. Systems that aid in diagnosing cancers are required to be highly-sensitive, so this metric focuses on the area under the ROC curve AND above 80% TRP. Hence, scores range from [0.0, 0.2].

```python
"""
2024 ISIC Challenge primary prize scoring metric

Given a list of binary labels, an associated list of prediction 
scores ranging from [0,1], this function produces, as a single value, 
the partial area under the receiver operating characteristic (pAUC) 
above a given true positive rate (TPR).
https://en.wikipedia.org/wiki/Partial_Area_Under_the_ROC_Curve.

(c) 2024 Nicholas R Kurtansky, MSKCC
"""

import numpy as np
import pandas as pd
import pandas.api.types
from sklearn.metrics import roc_curve, auc, roc_auc_score

class ParticipantVisibleError(Exception):
    pass


def score(solution: pd.DataFrame, submission: pd.DataFrame, row_id_column_name: str, min_tpr: float=0.80) -> float:
    '''
    2024 ISIC Challenge metric: pAUC
    
    Given a solution file and submission file, this function returns the
    the partial area under the receiver operating characteristic (pAUC) 
    above a given true positive rate (TPR) = 0.80.
    https://en.wikipedia.org/wiki/Partial_Area_Under_the_ROC_Curve.
    
    (c) 2024 Nicholas R Kurtansky, MSKCC

    Args:
        solution: ground truth pd.DataFrame of 1s and 0s
        submission: solution dataframe of predictions of scores ranging [0, 1]

    Returns:
        Float value range [0, max_fpr]
    '''

    del solution[row_id_column_name]
    del submission[row_id_column_name]

    # check submission is numeric
    if not pandas.api.types.is_numeric_dtype(submission.values):
        raise ParticipantVisibleError('Submission target column must be numeric')

    # rescale the target. set 0s to 1s and 1s to 0s (since sklearn only has max_fpr)
    v_gt = abs(np.asarray(solution.values)-1)
    
    # flip the submissions to their compliments
    v_pred = -1.0*np.asarray(submission.values)

    max_fpr = abs(1-min_tpr)

    # using sklearn.metric functions: (1) roc_curve and (2) auc
    fpr, tpr, _ = roc_curve(v_gt, v_pred, sample_weight=None)
    if max_fpr is None or max_fpr == 1:
        return auc(fpr, tpr)
    if max_fpr <= 0 or max_fpr > 1:
        raise ValueError("Expected min_tpr in range [0, 1), got: %r" % min_tpr)
        
    # Add a single point at max_fpr by linear interpolation
    stop = np.searchsorted(fpr, max_fpr, "right")
    x_interp = [fpr[stop - 1], fpr[stop]]
    y_interp = [tpr[stop - 1], tpr[stop]]
    tpr = np.append(tpr[:stop], np.interp(max_fpr, x_interp, y_interp))
    fpr = np.append(fpr[:stop], max_fpr)
    partial_auc = auc(fpr, tpr)

#     # Equivalent code that uses sklearn's roc_auc_score
#     v_gt = abs(np.asarray(solution.values)-1)
#     v_pred = np.array([1.0 - x for x in submission.values])
#     max_fpr = abs(1-min_tpr)
#     partial_auc_scaled = roc_auc_score(v_gt, v_pred, max_fpr=max_fpr)
#     # change scale from [0.5, 1.0] to [0.5 * max_fpr**2, max_fpr]
#     # https://math.stackexchange.com/questions/914823/shift-numbers-into-a-different-range
#     partial_auc = 0.5 * max_fpr**2 + (max_fpr - 0.5 * max_fpr**2) / (1.0 - 0.5) * (partial_auc_scaled - 0.5)
    
    return(partial_auc)


```

# Data 

Binary class {0: benign, 1: malignant}.
Benign : not harmful in effect.
Malignant : cancerous tumor

The dataset consists of diagnostically labelled images with additional metadata. The images are JPEGs. The associated .csv file contains a binary diagnostic label (target), potential input variables (e.g. age_approx, sex, anatom_site_general, etc.), and additional attributes (e.g. image source and precise diagnosis).

In this challenge we are differentiating benign from malignant cases. For each image (isic_id) you are assigning the probability (target) ranging [0, 1] that the case is malignant.

The SLICE-3D dataset - skin lesion image crops extracted from 3D TBP for skin cancer detection
To mimic non-dermoscopic images, this competition uses standardized cropped lesion-images of lesions from 3D Total Body Photography (TBP). Vectra WB360, a 3D TBP product from Canfield Scientific, captures the complete visible cutaneous surface area in one macro-quality resolution tomographic image. An AI-based software then identifies individual lesions on a given 3D capture. This allows for the image capture and identification of all lesions on a patient, which are exported as individual 15x15 mm field-of-view cropped photos. The dataset contains every lesion from a subset of thousands of patients seen between the years 2015 and 2024 across nine institutions and three continents.

The following are examples from the training set. 'Strongly-labelled tiles' are those whose labels were derived through histopathology assessment. 'Weak-labelled tiles' are those who were not biopsied and were considered 'benign' by a doctor.
![table](/assets/isic_3.jpg)


# Feature Engineering




### New Feature Descriptions for ISIC 2024 Challenge

1. **Lesion Size Ratio**: The ratio of the minor axis length to the longest diameter of the lesion.
   ```python
   df["lesion_size_ratio"] = df["tbp_lv_minorAxisMM"] / df["clin_size_long_diam_mm"]
   ```

2. **Lesion Shape Index**: The area-to-perimeter ratio that gives insights into the shape complexity.
   ```python
   df["lesion_shape_index"] = df["tbp_lv_areaMM2"] / (df["tbp_lv_perimeterMM"] ** 2)
   ```

3. **Hue Contrast**: The absolute difference in hue values between the lesion and its surroundings.
   ```python
   df["hue_contrast"] = (df["tbp_lv_H"] - df["tbp_lv_Hext"]).abs()
   ```

4. **Luminance Contrast**: The absolute difference in luminance between the lesion and surrounding skin.
   ```python
   df["luminance_contrast"] = (df["tbp_lv_L"] - df["tbp_lv_Lext"]).abs()
   ```

5. **Lesion Color Difference**: The Euclidean distance in the color space, indicating variation in color.
   ```python
   df["lesion_color_difference"] = np.sqrt(df["tbp_lv_deltaA"] ** 2 + df["tbp_lv_deltaB"] ** 2 + df["tbp_lv_deltaL"] ** 2)
   ```

6. **Border Complexity**: A combination of normalized border and symmetry metrics to describe irregularity.
   ```python
   df["border_complexity"] = df["tbp_lv_norm_border"] + df["tbp_lv_symm_2axis"]
   ```

7. **Color Uniformity**: The ratio of color standard deviation to the radial color variation, assessing uniformity.
   ```python
   df["color_uniformity"] = df["tbp_lv_color_std_mean"] / df["tbp_lv_radial_color_std_max"]
   ```

8. **3D Position Distance**: The spatial distance in the 3D plane.
   ```python
   df["3d_position_distance"] = np.sqrt(df["tbp_lv_x"] ** 2 + df["tbp_lv_y"] ** 2 + df["tbp_lv_z"] ** 2)
   ```

9. **Perimeter-to-Area Ratio**: Measures lesion compactness by comparing perimeter to area.
   ```python
   df["perimeter_to_area_ratio"] = df["tbp_lv_perimeterMM"] / df["tbp_lv_areaMM2"]
   ```

10. **Lesion Visibility Score**: A combination of lesion contrast and color attributes for visibility assessment.
    ```python
    df["lesion_visibility_score"] = df["tbp_lv_deltaLBnorm"] + df["tbp_lv_norm_color"]
    ```

11. **Combined Anatomical Site**: Concatenates anatomical site information and lesion location.
    ```python
    df["combined_anatomical_site"] = df["anatom_site_general"] + "_" + df["tbp_lv_location"]
    ```

12. **Symmetry-Border Consistency**: The interaction between symmetry and border characteristics.
    ```python
    df["symmetry_border_consistency"] = df["tbp_lv_symm_2axis"] * df["tbp_lv_norm_border"]
    ```

13. **Color Consistency**: Evaluates how consistent the lesion's internal and external colors are.
    ```python
    df["color_consistency"] = df["tbp_lv_stdL"] / df["tbp_lv_Lext"]
    ```

14. **Size-Age Interaction**: Combines lesion size and patient age for demographic insights.
    ```python
    df["size_age_interaction"] = df["clin_size_long_diam_mm"] * df["age_approx"]
    ```

15. **Hue-Color Standard Interaction**: Interactions between hue and color standard deviation to analyze color complexity.
    ```python
    df["hue_color_std_interaction"] = df["tbp_lv_H"] * df["tbp_lv_color_std_mean"]
    ```

16. **Lesion Severity Index**: Combines border, color, and eccentricity metrics to represent lesion severity.
    ```python
    df["lesion_severity_index"] = (df["tbp_lv_norm_border"] + df["tbp_lv_norm_color"] + df["tbp_lv_eccentricity"]) / 3
    ```

17. **Shape Complexity Index**: Summarizes border complexity and lesion shape attributes for complexity analysis.
    ```python
    df["shape_complexity_index"] = df["border_complexity"] + df["lesion_shape_index"]
    ```

18. **Color Contrast Index**: Sum of the differences in color attributes for measuring contrast.
    ```python
    df["color_contrast_index"] = df["tbp_lv_deltaA"] + df["tbp_lv_deltaB"] + df["tbp_lv_deltaL"] + df["tbp_lv_deltaLBnorm"]
    ```

19. **Log Lesion Area**: The logarithmic transformation of lesion area to reduce skewness in data.
    ```python
    df["log_lesion_area"] = np.log(df["tbp_lv_areaMM2"] + 1)
    ```

20. **Normalized Lesion Size**: Lesion size normalized by patient age for a balanced size metric.
    ```python
    df["normalized_lesion_size"] = df["clin_size_long_diam_mm"] / df["age_approx"]
    ```

21. **Mean Hue Difference**: The average of hue values between the lesion and surroundings.
    ```python
    df["mean_hue_difference"] = (df["tbp_lv_H"] + df["tbp_lv_Hext"]) / 2
    ```

22. **Standard Deviation Contrast**: The standard deviation of color differences to capture variability in color contrast.
    ```python
    df["std_dev_contrast"] = np.sqrt((df["tbp_lv_deltaA"] ** 2 + df["tbp_lv_deltaB"] ** 2 + df["tbp_lv_deltaL"] ** 2) / 3)
    ```

23. **Color-Shape Composite Index**: A composite score that blends color, shape, and symmetry factors.
    ```python
    df["color_shape_composite_index"] = (df["tbp_lv_color_std_mean"] + df["tbp_lv_area_perim_ratio"] + df["tbp_lv_symm_2axis"]) / 3
    ```

24. **3D Lesion Orientation**: The angle of the lesion in 3D space.
    ```python
    df["3d_lesion_orientation"] = np.arctan2(df_train["tbp_lv_y"], df_train["tbp_lv_x"])
    ```

25. **Overall Color Difference**: The average of color differences across multiple channels.
    ```python
    df["overall_color_difference"] = (df["tbp_lv_deltaA"] + df["tbp_lv_deltaB"] + df["tbp_lv_deltaL"]) / 3
    ```

26. **Symmetry-Perimeter Interaction**: The interaction between symmetry and perimeter for irregularity assessment.
    ```python
    df["symmetry_perimeter_interaction"] = df["tbp_lv_symm_2axis"] * df["tbp_lv_perimeterMM"]
    ```

27. **Comprehensive Lesion Index**: A composite index summarizing lesion area, perimeter, color, and symmetry.
    ```python
    df["comprehensive_lesion_index"] = (df["tbp_lv_area_perim_ratio"] + df["tbp_lv_eccentricity"] + df["tbp_lv_norm_color"] + df["tbp_lv_symm_2axis"]) / 4
    ```

 I produced them using ChatGPT. I provided the feature descriptions and tried to brainstorm ideas.

Encoding is a required pre-processing step when working with categorical data for machine learning algorithms.
Numerical data, as its name suggests, involves features that are only composed of numbers, such as integers or floating-point values.
Categorical data are variables that contain label values rather than numeric values.
The number of possible values is often limited to a fixed set.
Nominal Variable (Categorical). Variable comprises a finite set of discrete values with no relationship between values.
Ordinal Variable. Variable comprises a finite set of discrete values with a ranked ordering between values.

In ordinal encoding, each unique category value is assigned an integer value.

OrdinalEncoder assigns each unique category in a feature to a unique integer based on the order they appear or a specified order. By default, it assigns integers in lexicographical order, but we can customize the ordering if needed.
1. Identify Unique Categories: For each categorical feature, identify all unique categories.
2. Assign Integers: Assign an integer to each category. The default is in lexicographical order, but this can be customized.
3. Transform Data: Replace each category in the dataset with its corresponding integer.
If the categorical data has an inherent order that isn't lexicographical, we can specify the order using the categories parameter.

When to Use OrdinalEncoder?
- Ordinal Categorical Features: When the categorical feature has a meaningful order (e.g., rating scales like "Poor", "Fair", "Good", "Very Good", "Excellent").
- Algorithms Sensitive to Integer Encoding: Some algorithms (e.g., tree-based models like Decision Trees, Random Forests) can handle integer-encoded categorical features without assuming a specific order.

categorical_columns = ["sex", "tbp_tile_type", "tbp_lv_location", "tbp_lv_location_simple","combined_anatomical_site"]

category_encoder = OrdinalEncoder(
    categories='auto',
    dtype=int,
    handle_unknown='use_encoded_value',
    unknown_value=-2,
    encoded_missing_value=-1,
)

```python
X_cat = category_encoder.fit_transform(new_train[categorical_columns])
```
