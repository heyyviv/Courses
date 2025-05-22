---
layout: post
title: " Normalisation "
date: 2024-08-24 01:43:18 +0530
categories: Deep Learning
---

Different Normalization Layers in Deep Learning

 In the process of training a neural network, we initialize the weights which are then updated as the training proceeds. For a certain random initialization, the outputs from one or more of the intermediate layers can be abnormally large. This leads to instability in the training process, which means the network will not learn anything useful during training.

 Batch and layer normalization are two strategies for training neural networks faster, without having to be overly cautious with initialization and other regularization techniques.

 If you proceed to train your model on such datasets with input features on different scales, you’ll notice that the neural network takes significantly longer to train because the gradient descent algorithm takes longer to converge when the input features are not all on the same scale. Additionally, such high values can also propagate through the layers of the network leading to the accumulation of large error gradients that make the training process unstable, called the problem of exploding gradients.

But why does this hamper the training process?

For each batch in the input dataset, the mini-batch gradient descent algorithm runs its updates. It updates the weights and biases (parameters) of the neural network so as to fit to the distribution seen at the input to the specific layer for the current batch.

It’s also possible that the input distribution at a particular layer keeps changing across batches. The seminal paper titled Batch 
Normalization: Accelerating Deep Network Training by Reducing Internal Covariate Shift by Sergey Ioffe and Christian Szegedy refers to 
this change in distribution of the input to a particular layer across batches as internal covariate shift. For instance, if the 
distribution of data at the input of layer K keeps changing across batches, the network will take longer to train.



Batch Normalization(BN)

Batch Normalization focuses on standardizing the inputs to any particular layer(i.e. activations from previous layers). Standardizing the inputs mean that inputs to any layer in the network should have approximately zero mean and unit variance. Mathematically, BN layer transforms each input in the current mini-batch by subtracting the input mean in the current mini-batch and dividing it by the standard deviation.

But each layer doesn’t need to expect inputs with zero mean and unit variance, but instead, probably the model might perform better with some other mean and variance. Hence the BN layer also introduces two learnable parameters γ and β.

The whole layer operation is as follows. It takes an input x_i and transforms it into y_i as described in the below table.

![BN](/assets/norm_1.jpg)

The question is how BN helps NN training? Intuitively, In gradient descent, the network calculates the gradient based on the current
 inputs to any layer and reduce the weights in the direction indicated by the gradient. But since the layers are stacked one after the 
 other, the data distribution of input to any particular layer changes too much due to slight update in weights of earlier layer, and 
 hence the current gradient might produce suboptimal signals for the network. But BN restricts the distribution of the input data to any 
 particular layer(i.e. the activations from the previous layer) in the network, which helps the network to produce better gradients for 
 weights update. Hence BN often provides a much stable and accelerated training regime.


However, forcing all the pre-activations to be zero and unit standard deviation across all batches can be too restrictive. It may be the case that the fluctuant distributions are necessary for the network to learn certain classes better.

To address this, batch normalization introduces two parameters: a scaling factor gamma (γ) and an offset beta (β). These are learnable parameters, so if the fluctuation in input distribution is necessary for the neural network to learn a certain class better, then the network learns the optimal values of gamma and beta for each mini-batch. The gamma and beta are learnable such that it’s possible to go back from the normalized pre-activations to the actual distributions that the pre-activations follow.

Two limitations of batch normalization can arise:

- In batch normalization, we use the batch statistics: the mean and standard deviation corresponding to the current mini-batch. However, when the batch size is small, the sample mean and sample standard deviation are not representative enough of the actual distribution and the network cannot learn anything meaningful.
- As batch normalization depends on batch statistics for normalization, it is less suited for sequence models. This is because, in sequence models, we may have sequences of potentially different lengths and smaller batch sizes corresponding to longer sequences.



![BN](/assets/norm_2.jpg)


Layer Normalization(LN)

LN normalizes the activations of each layer independently across all features. This means that the mean and variance of the activations are calculated for each layer separately, and then the activations are scaled and shifted to have a standard normal distribution (mean of 0 and variance of 1).

In LN, the “covariate shift” problem can be reduced by fixing the mean and the variance of the summed inputs within each layer.

Layer Normalization was proposed by researchers Jimmy Lei Ba, Jamie Ryan Kiros, and Geoffrey E. Hinton. In layer normalization, all neurons in a particular layer effectively have the same distribution across all features for a given input.

For example, if each input has d features, it’s a d-dimensional vector. If there are B elements in a batch, the normalization is done along the length of the d-dimensional vector and not across the batch of size B.

Normalizing across all features but for each of the inputs to a specific layer removes the dependence on batches. This makes layer normalization well suited for sequence models such as transformers and recurrent neural networks (RNNs) that were popular in the pre-transformer era.

Here’s an example showing the computation of the mean and variance for layer normalization. We consider the example of a mini-batch containing three input samples, each with four features.

![LN](/assets/norm_3.jpg)
Normalisation acroos feature independent of each sample


Batch Normalization vs Layer Normalization

- Batch normalization normalizes each feature independently across the mini-batch. Layer normalization normalizes each of the inputs in the batch independently across all features.
- As batch normalization is dependent on batch size, it’s not effective for small batch sizes. Layer normalization is independent of the batch size, so it can be applied to batches with smaller sizes as well.
- Batch normalization requires different processing at training and inference times. As layer normalization is done along the length of input to a specific layer, the same set of operations can be used at both training and inference times.



Instance Normalization (IN)

Instance normalization is another term for contrast normalization, which was first coined in the StyleNet paper. Both names reveal some information about this technique. Instance normalization tells us that it operates on a single sample. On the other hand, contrast normalization says that it normalizes the contrast between the spatial elements of a sample. Given a Convolution Neural Network (CNN), we can also say that IN performs intensity normalization across the width and height of a single feature map of a single example.

To clarify how IN works, let’s consider sample feature maps that constitute an input tensor to the IN layer. Let x be that tensor consisting of a batch of N images. Each of these images has C feature maps or channels with height H and weight W. Therefore, x \in \R^{N\times C\times H\times W} is a four-dimensional tensor. In instance normalization, we consider one training sample and feature map (specified in red in the figure) and take the mean and variance over its spatial locations (W and H):

![IN](/assets/norm_4.jpg)

Group Normalization

Group normalization is particularly helpful when dealing with small or variable batches. It works by dividing the channels in a layer into groups and normalizing the resources separately in each group.

In this scenario, a group is simply an independent subset of the channels. Therefore, organize the channels into different groups and calculate the mean and standard deviation along the axes.

![IN](/assets/norm_5.jpg)