---
layout: post
title: "Inception"
date: 2024-08-24 01:43:18 +0530
categories: Deep Learning
---

Going deeper with convolutions

Sparse matrices: This term refers to any matrix that is mainly composed of zeros. (In contrast, dense matrices are mostly composed of non-zero values). Matrix is a matrix, of course, but by dealing with sparse matrices as a special data structure, we can save computational resources. In the notoriously resource-hungry world of machine learning, its importance cannot be overstated. Sparse matrices can be saved as dictionaries of keys, lists of lists, or more specialized structures, such as compressed sparse rows.

1x1 convolution: This layer is added to the neural network to keep the computational cost maintainable. Normally, the matrix is downsampled (through max-pooling) to achieve this, however, we cannot perform downsampling too many times, otherwise, information loss accumulates, which we want to avoid. 1x1 convolution solves this problem by pooling not inside a single channel, but across the channels themselves (Figure 1).

GoogLeNet’s deep learning model was deeper than all the previous models released, with 22 layers in total. Increasing the depth of the Machine Learning model is intuitive, as deeper models tend to have more learning capacity and as a result, this increases the performance of a model. However, this is only possible if we can solve the vanishing gradient problem.

A large kernel size is used to capture a global distribution of the image while a small kernel size is used to capture more local information.

When designing a deep learning model, one needs to decide what convolution filter size to use (whether it should be 3×3, 5×5, or 1×3) as it affects the model’s learning and performance, and when to max pool the layers. However, the inception module, the key innovation introduced by a team of Google researchers solved this problem creatively. Instead of deciding what filter size to use and when to perform a max pooling operation, they combined multiple convolution filters.

Stacking multiple convolution filters together instead of just one increases the parameter count many times. However, GoogLeNet demonstrated by using the inception module that depth and width in a neural network could be increased without exploding computations.  We will investigate the inception module in depth.

![Inception](/assets/inception_6.jpg)

![Inception](/assets/inception_7.jpg)

Moreover, the architecture is relatively deep with 22 layers, however, the model maintains computational efficiency despite the increase in the number of layers.

Here are the key features of GoogLeNet:

- Inception Module
- The 1×1 Convolution
- Global Average Pooling
- Auxiliary Classifiers for Training

Inception network architecture makes it possible to use filters of multiple sizes without increasing the depth of the network. The different filters are added parallelly instead of being fully connected one after the other.

![Inception](/assets/inception_1.jpg)
This is known as the naive version of the inception model. The problem with this model was the huge number of parameters. To mitigate the same, they came up with the below architecture.
![Inception](/assets/inception_2.jpg)

How does this architecture reduce dimensionality?
Adding a 1X1 convolution before a 5X5 convolution would reduce the number of channels of the image when it is provided as an input to the 5X5 convolution, in turn reducing the number of parameters and the computational requirement.

![Inception](/assets/inception_3.jpg)

![Inception](/assets/inception_4.jpg)

The Inception Module is the building block of GoogLeNet, as the entire model is made by stacking Inception Modules. Here are the key features of it:

- Multi-Level Feature Extraction: The main idea of the inception module is that it consists of multiple pooling and convolution operations with different sizes (3×3, 5×5) in parallel, instead of using just one filter of a single size.
- Dimension Reduction: However, as we discussed earlier, stacking multiple layers of convolution results in increased computations. To overcome this, the researchers incorporate 1×1 convolution before feeding the data into 3×3 or 5×5 convolutions. We can also refer to this as dimensionality reduction.

Benefits
- Parameter Efficiency: By using 1×1 convolutions, the module reduces dimensionality before applying the more expensive 3×3 and 5×5 convolutions and pooling operations.
- Increased Representation: By incorporating filters of varying sizes and more layers, the network captures a wide range of features in the input data. This results in better representation.
- Enhancing Feature Combination: The 1×1 convolution is also called network in the network. This means that each layer is a micro-neural network that learns to abstract the data before the main convolution filters are applied.

Global Average Pooling
Global Average Pooling is a Convolutional Neural Networks (CNN) technique in the place of fully connected layers at the end part of the network. This method reduces the total number of parameters and minimizes overfitting.

For example, consider you have a feature map with dimensions 10,10, 32 (Width, Height, Channels).

Global Average Pooling performs an average operation across the Width and Height of each filter channel separately. This reduces the feature map to a vector that is equal to the size of the number of channels.

The output vector captures the most prominent features by summarizing the activation of each channel across the entire feature map. Here our output vector is of the length 32, which is equal to the number of channels.

Benefits of Global Average Pooling
- Reduced Dimensionality: GAP significantly reduces the number of parameters in the network, making it efficient and faster during training and inference. Due to the absence of trainable parameters, the model is less prone to overfitting.
- Robustness to Spatial Variations: The entire feature map is summarized, as a result, GAP is less sensitive to small spatial shifts in the object’s location within the image.
- Computationally Efficient: It’s a simple operation in comparison to a set of fully connected layers.

Auxiliary Classifiers for Training GoogleNet
These are intermediate classifiers found on the side of the network. One important thing to note is that these are only used during training and in the inference, these are omitted.

Auxiliary classifiers help overcome the challenges of training very Deep Neural Networks, and vanishing gradients (when the gradients turn into extremely small values).

In the GoogLeNet architecture, there are two auxiliary classifiers in the network. They are placed strategically, where the depth of the feature extracted is sufficient to make a meaningful impact, but before the final prediction from the output classifier.

More details on the structure of each auxiliary classifier:

An average pooling layer with a 5×5 window and stride 3.
A 1×1 convolution for dimension reduction with 128 filters.
Two fully connected layers, the first layer with 1024 units, followed by a dropout layer and the final layer corresponding to the number of classes in the task.
A SoftMax layer to output the prediction probabilities.
During training, the loss calculated from each auxiliary classifier is weighted and added to the total loss of the network. In the original paper, it is set to 0.3.

These auxiliary classifiers help the gradient to flow and not diminish too quickly, as it propagates back through the deeper layers. This is what makes training a Deep Neural Network like GoogLeNet possible.

Moreover, the auxiliary classifiers also help with model regularization. Since each classifier contributes to the final output, as a result, the network distributes its learning across different parts of the network. This distribution prevents the network from relying too heavily on specific features or layers, which reduces the chances of overfitting.


What is different in the Inception V3 network from the inception V1 network?

Inception V3 is an extension of the V1 module, it uses techniques like factorizing larger convolutions to smaller convolutions (say a 5X5 convolution is factorized into two 3X3 convolutions) and asymmetric factorizations (example: factorizing a 3X3 filter into a 1X3 and 3X1 filter).
These factorizations are done with the aim of reducing the number of parameters being used at every inception module. Below is an image of the inception V3 module.

![Inception](/assets/inception_5.jpg)



