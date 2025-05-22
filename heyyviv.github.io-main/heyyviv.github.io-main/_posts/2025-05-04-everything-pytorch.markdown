---
layout: post
title: "Everything PyTorch"
date: 2025-05-04 01:43:18 +0530
categories: deep_learning
---

PyTorch feels like NumPy, but with GPU acceleration and automatic computation of gradients, which makes it suitable for calculating backward pass data automatically starting from a forward expression.

Best feature about pytorch is :
- PyTorch provides allows tensors to keep track of the operations performed on them and to compute derivatives of an output with respect to any of its inputs analytically via backpropagation. This capability is provided natively by tensors and further refined in torch.autograd

PyTorch defaults to an immediate execution model (eager mode). Whenever an instruction involving PyTorch is executed by the Python interpreter, the corresponding operation is immediately carried out by the underlying C++ or CUDA implementation. As more instructions operate on tensors, more operations are executed by the backend implementation. This process is as fast as it typically can be on the C++ side, but it incurs the cost of calling that implementation through Python. This cost is minute, but it adds up.

# Tensor
Tensor is array that can access by an index or set of indices.
Many types of data—from images to time series, audio, and even sentences—can be represented by tensors.

```python 
import torch
a = torch.ones(3)
a
o/p:
tensor([1., 1., 1.]) 
```
PyTorch tensors or NumPy arrays,  are views over (typically) contiguous memory blocks containing unboxed C numeric types, not Python objects. 
Python lists or tuples of numbers are collections of Python objects that are individually allocated in memory.

![Tensor](/assets/pytorch_1.png)
The dtype argument to tensor constructors (that is, functions such as tensor, zeros, and ones) specifies the numerical data type that will be contained in the tensor.

![Tensor](/assets/pytorch_2.jpg)

PyTorch uses pickle under the hood to serialize the tensor object, as well as dedicated serialization code for the storage. 

save: 
torch.save(points, '../data/p1ch3/ourpoints.t')
or
with open('../data/p1ch3/ourpoints.t','wb') as f:
    torch.save(points, f)

Loading your points:
points = torch.load('../data/p1ch3/ourpoints.t')
or
with open('../data/p1ch3/ourpoints.t','rb') as f:
    points = torch.load(f)

In addition to the dtype, a PyTorch tensor has a notion of device, which is where on the computer the tensor data is being placed.
points_gpu = torch.tensor([[1.0, 4.0], [2.0, 1.0], [3.0, 4.0]],device='cuda')
You could instead copy a tensor created on the CPU to the GPU by using the to method:
points_gpu = points.to(device='cuda')

Tabular data is heterogenous it contain date,category,time,number etc.
Your first job as a deep learning practitioner, therefore, is to encode heterogenous, real-world data in a tensor of floating-point numbers, ready for consumption by a neural network.

data = wineq[:, :-1] 
select all rows except the last column.
target = wineq[:, -1]
select all rows and only last column.

attributes of a tensor:
```python
tensor = torch.rand(3,4)
print(f"Shape of tensor: {tensor.shape}")
print(f"Datatype of tensor: {tensor.dtype}")
print(f"Device tensor is stored on: {tensor.device}")
```
By default, tensors are created on the CPU. We need to explicitly move tensors to the accelerator using .to method (after checking for accelerator availability). Keep in mind that copying large tensors across devices can be expensive in terms of time and memory!

Joining tensors You can use torch.cat to concatenate a sequence of tensors along a given dimension. 
```python
tensor = torch.ones(4, 4)
torch.cat([tensor,tensor,tensor],dim=1)
torch.cat([tensor,tensor,tensor],dim=0)
```
op:
1.tensor([[1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.],
        [1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.],
        [1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.],
        [1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.]])
2.tensor([[1., 1., 1., 1.],
        [1., 1., 1., 1.],
        [1., 1., 1., 1.],
        [1., 1., 1., 1.],
        [1., 1., 1., 1.],
        [1., 1., 1., 1.],
        [1., 1., 1., 1.],
        [1., 1., 1., 1.],
        [1., 1., 1., 1.],
        [1., 1., 1., 1.],
        [1., 1., 1., 1.],
        [1., 1., 1., 1.]])

matrix multiplication:  (tensor.T returns the transpose of a tensor)
y1 = tensor @ tensor.T
y2 = tensor.matmul(tensor.T)
element wise multiplication
z1 = tensor * tensor
z2 = tensor.mul(tensor)
Tensors on the CPU and NumPy arrays can share their underlying memory locations, and changing one will change the other.
## Tensor Memory Storage

### Continuous Storage
Tensors are stored in memory in a continuous manner, just like arrays. The elements occupy sequential memory locations for efficient access.

### Tensor Slicing and Views
When we create a slice of a tensor, such as:

```python
t2 = t1[2, :]  # Extract row 2 with all columns from an m×n tensor
```

The new tensor `t2` doesn't receive its own separate memory allocation. Instead:

- `t2` creates an abstract view of the original data
- It references the same memory as the original tensor
- It uses a stride-based access pattern

## Stride-Based Memory Access
For the row slice example:
- The system calculates an offset from the base address to reach row 2
- It sets a stride value of `m` (the row length of the original tensor)
- When accessing elements sequentially in `t2`, the system jumps by the stride value
- Each element access follows: base_address + initial_offset + (index × stride)

This approach is memory-efficient since it avoids copying data, making tensor operations faster and reducing overall memory usage.


There may be multiple tensors which share the same storage. Storage defines the dtype and physical size of the tensor, while each tensor records the sizes, strides and offset, defining the logical interpretation of the physical memory.
when we  call torch.mm, two dispatches happen:
- The first dispatch is based on the device type and layout of a tensor: e.g., whether or not it is a CPU tensor or a CUDA tensor (and also, e.g., whether or not it is a strided tensor or a sparse one). This is a dynamic dispatch: it's a virtual function call.It should make sense that we need to do a dispatch here: the implementation of CPU matrix multiply is quite different from a CUDA implementation. It is a dynamic dispatch because these kernels may live in separate libraries.if we want to get into a library that we don't have a direct dependency on, we have to dynamic dispatch your way there.
- The second dispatch is a dispatch on the dtype in question. This dispatch is just a simple switch-statement for whatever dtypes a kernel chooses to support. Upon reflection, it should also make sense that we need to a dispatch here: the CPU code (or CUDA code, as it may) that implements multiplication on float is different from the code for int. It stands to reason you need separate kernels for each dtype

There is the trinity three parameters which uniquely determine what a tensor is:
- The device, the description of where the tensor's physical memory is actually stored, e.g., on a CPU, on an NVIDIA GPU (cuda), or perhaps on an AMD GPU (hip) or a TPU (xla). The distinguishing characteristic of a device is that it has its own allocator, that doesn't work with any other device.
- The layout, which describes how we logically interpret this physical memory. The most common layout is a strided tensor, but sparse tensors have a different layout involving a pair of tensors, one for indices, and one for data; MKL-DNN tensors may have even more exotic layout, like blocked layout, which can't be represented using merely strides.
- The dtype, which describes what it is that is actually stored in each element of the tensor. This could be floats or integers, or it could be, for example, quantized integers.



# Datasets & DataLoaders
Dataset stores the samples and their corresponding labels, and DataLoader wraps an iterable around the Dataset to enable easy access to the samples.

custom dataset:
```python
import os
import pandas as pd
from torchvision.io import read_image

class CustomImageDataset(Dataset):
    def __init__(self, annotations_file, img_dir, transform=None, target_transform=None):
        self.img_labels = pd.read_csv(annotations_file)
        self.img_dir = img_dir
        self.transform = transform
        self.target_transform = target_transform

    def __len__(self):
        return len(self.img_labels)

    def __getitem__(self, idx):
        img_path = os.path.join(self.img_dir, self.img_labels.iloc[idx, 0])
        image = read_image(img_path)
        label = self.img_labels.iloc[idx, 1]
        if self.transform:
            image = self.transform(image)
        if self.target_transform:
            label = self.target_transform(label)
        return image, label
```
The __init__ function is run once when instantiating the Dataset object. We initialize the directory containing the images, the annotations file, and both transforms.
The __len__ function returns the number of samples in our dataset.
The __getitem__ function loads and returns a sample from the dataset at the given index idx. Based on the index, it identifies the image’s location on disk, converts that to a tensor using read_image, retrieves the corresponding label from the csv data in self.img_labels, calls the transform functions on them (if applicable), and returns the tensor image and corresponding label in a tuple.

The Dataset retrieves our dataset’s features and labels one sample at a time. While training a model, we typically want to pass samples in “minibatches”, reshuffle the data at every epoch to reduce model overfitting, and use Python’s multiprocessing to speed up data retrieval.

DataLoader is an iterable that abstracts this complexity for us in an easy API.
```python
from torch.utils.data import DataLoader

train_dataloader = DataLoader(training_data, batch_size=64, shuffle=True)
test_dataloader = DataLoader(test_data, batch_size=64, shuffle=True)
```

# Build the Neural Network
The torch.nn namespace provides all the building blocks you need to build your own neural network.
```python
class NeuralNetwork(nn.Module):
    def __init__(self):
        super().__init__()
        self.flatten = nn.Flatten()
        self.linear_relu_stack = nn.Sequential(
            nn.Linear(28*28, 512),
            nn.ReLU(),
            nn.Linear(512, 512),
            nn.ReLU(),
            nn.Linear(512, 10),
        )

    def forward(self, x):
        x = self.flatten(x)
        logits = self.linear_relu_stack(x)
        return logits
```
We create an instance of NeuralNetwork, and move it to the device, and print its structure.
model = NeuralNetwork().to(device)
print(model)
out: NeuralNetwork(
  (flatten): Flatten(start_dim=1, end_dim=-1)
  (linear_relu_stack): Sequential(
    (0): Linear(in_features=784, out_features=512, bias=True)
    (1): ReLU()
    (2): Linear(in_features=512, out_features=512, bias=True)
    (3): ReLU()
    (4): Linear(in_features=512, out_features=10, bias=True)
  )
)
nn.Sequential is an ordered container of modules. The data is passed through all the modules in the same order as defined.
Many layers inside a neural network are parameterized, i.e. have associated weights and biases that are optimized during training. Subclassing nn.Module automatically tracks all fields defined inside your model object, and makes all parameters accessible using your model’s parameters() or named_parameters() methods.

# Automatic Differentiation
PyTorch has a built-in differentiation engine called torch.autograd. It supports automatic computation of gradient for any computational graph.

```python
import torch
x = torch.ones(5)  # input tensor
y = torch.zeros(3)  # expected output
w = torch.randn(5, 3, requires_grad=True)
b = torch.randn(3, requires_grad=True)
z = torch.matmul(x, w)+b
loss = torch.nn.functional.binary_cross_entropy_with_logits(z, y)
```
![Gradient](/assets/pytorch_3.jpg)

By default, all tensors with requires_grad=True are tracking their computational history and support gradient computation. However, there are some cases when we do not need to do that, for example, when we have trained the model and just want to apply it to some input data, i.e. we only want to do forward computations through the network. We can stop tracking computations by surrounding our computation code with torch.no_grad() block:
```python
with torch.no_grad():
    z = torch.matmul(x, w)+b
```
There are reasons you might want to disable gradient tracking:
- To mark some parameters in your neural network as frozen parameters.
- To speed up computations when you are only doing forward pass, because computations on tensors that do not track gradients would be more efficient.

# Optimizing Model Parameters 
Hyperparameters are adjustable parameters that let you control the model optimization process. Different hyperparameter values can impact model training and convergence rates.
We define the following hyperparameters for training:
- Number of Epochs - the number of times to iterate over the dataset
- Batch Size - the number of data samples propagated through the network before the parameters are updated
- Learning Rate - how much to update models parameters at each batch/epoch. Smaller values yield slow learning speed, while large values may result in unpredictable behavior during training.

Inside the training loop, optimization happens in three steps:
- Call optimizer.zero_grad() to reset the gradients of model parameters. Gradients by default add up; to prevent double-counting, we explicitly zero them at each iteration.
- Backpropagate the prediction loss with a call to loss.backward(). PyTorch deposits the gradients of the loss w.r.t. each parameter.
- Once we have our gradients, we call optimizer.step() to adjust the parameters by the gradients collected in the backward pass.

Implementation
```python
def train_loop(dataloader, model, loss_fn, optimizer):
    size = len(dataloader.dataset)
    # Set the model to training mode - important for batch normalization and dropout layers
    # Unnecessary in this situation but added for best practices
    model.train()
    for batch, (X, y) in enumerate(dataloader):
        # Compute prediction and loss
        pred = model(X)
        loss = loss_fn(pred, y)

        # Backpropagation
        loss.backward()
        optimizer.step()
        optimizer.zero_grad()

        if batch % 100 == 0:
            loss, current = loss.item(), batch * batch_size + len(X)
            print(f"loss: {loss:>7f}  [{current:>5d}/{size:>5d}]")


def test_loop(dataloader, model, loss_fn):
    # Set the model to evaluation mode - important for batch normalization and dropout layers
    # Unnecessary in this situation but added for best practices
    model.eval()
    size = len(dataloader.dataset)
    num_batches = len(dataloader)
    test_loss, correct = 0, 0

    # Evaluating the model with torch.no_grad() ensures that no gradients are computed during test mode
    # also serves to reduce unnecessary gradient computations and memory usage for tensors with requires_grad=True
    with torch.no_grad():
        for X, y in dataloader:
            pred = model(X)
            test_loss += loss_fn(pred, y).item()
            correct += (pred.argmax(1) == y).type(torch.float).sum().item()

    test_loss /= num_batches
    correct /= size
    print(f"Test Error: \n Accuracy: {(100*correct):>0.1f}%, Avg loss: {test_loss:>8f} \n")
```

To use torch.optim you have to construct an optimizer object that will hold the current state and will update the parameters based on the computed gradients.
To construct an Optimizer you have to give it an iterable containing the parameters (all should be Parameter s) or named parameters (tuples of (str, Parameter)) to optimize. Then, you can specify optimizer-specific options such as the learning rate, weight decay, etc.
```python
optimizer = optim.SGD(model.parameters(), lr=0.01, momentum=0.9)
```
*               or
```python
optimizer = optim.SGD(model.named_parameters(), lr=0.01, momentum=0.9)
```
Optimizer s also support specifying per-parameter options.
```python
optim.SGD([
                {'params': model.base.parameters(), 'lr': 1e-2},
                {'params': model.classifier.parameters()}
            ], lr=1e-3, momentum=0.9)

optim.SGD([
                {'params': model.base.named_parameters(), 'lr': 1e-2},
                {'params': model.classifier.named_parameters()}
            ], lr=1e-3, momentum=0.9)
```
This means that model.base’s parameters will use a learning rate of 1e-2, whereas model.classifier’s parameters will stick to the default learning rate of 1e-3. Finally a momentum of 0.9 will be used for all parameters.

Also consider the following example related to the distinct penalization of parameters. Remember that parameters() returns an iterable that contains all learnable parameters, including biases and other parameters that may prefer distinct penalization. To address this, one can specify individual penalization weights for each parameter group:
```python
bias_params = [p for name, p in self.named_parameters() if 'bias' in name]
others = [p for name, p in self.named_parameters() if 'bias' not in name]

optim.SGD([
                {'params': others},
                {'params': bias_params, 'weight_decay': 0}
            ], weight_decay=1e-2, lr=1e-2)
```
In this manner, bias terms are isolated from non-bias terms, and a weight_decay of 0 is set specifically for the bias terms, as to avoid any penalization for this group.
Weight decay is typically set to 0 for bias terms in neural networks for several key reasons:
- Overfitting usually occurs when the model becomes too sensitive to small changes in input data. The bias parameters don't contribute to the curvature of the model, so regularizing them provides little benefit in preventing overfitting
- While weights determine the slopes of activation functions, biases only affect the position of activation functions in space. Their optimal values depend on the weights and should be adjusted without regularization

All optimizers implement a step() method, that updates the parameters
```python
for input, target in dataset:
    optimizer.zero_grad()
    output = model(input)
    loss = loss_fn(output, target)
    loss.backward()
    optimizer.step()
```


# Save and Load the Model
PyTorch models store the learned parameters in an internal state dictionary, called state_dict. These can be persisted via the torch.save method:
```python
model = models.vgg16(weights='IMAGENET1K_V1')
torch.save(model.state_dict(), 'model_weights.pth')
```
To load model weights, you need to create an instance of the same model first, and then load the parameters using load_state_dict() method.

```python
model = models.vgg16() # we do not specify ``weights``, i.e. create untrained model
model.load_state_dict(torch.load('model_weights.pth', weights_only=True))
model.eval()
```
When loading model weights, we needed to instantiate the model class first, because the class defines the structure of a network. We might want to save the structure of this class together with the model, in which case we can pass model (and not model.state_dict()) to the saving function:
torch.save(model, 'model.pth')
As described in Saving and loading torch.nn.Modules, saving state_dict is considered the best practice. However, below we use weights_only=False because this involves loading the model, which is a legacy use case for torch.save.
model = torch.load('model.pth', weights_only=False),

there are only four directories you really need to know about:
- First, torch/ contains what you are most familiar with: the actual Python modules that you import and use. This stuff is Python code and easy to hack on (just make a change and see what happens). However, lurking not too deep below the surface is...
- torch/csrc/, the C++ code that implements what you might call the frontend of PyTorch. In more descriptive terms, it implements the binding code that translates between the Python and C++ universe, and also some pretty important pieces of PyTorch, like the autograd engine and the JIT compiler. It also contains the C++ frontend code.
- aten/, short for "A Tensor Library" (coined by Zachary DeVito), is a C++ library that implements the operations of Tensors. If you're looking for where some kernel code lives, chances are it's in ATen. ATen itself bifurcates into two neighborhoods of operators: the "native" operators, which are modern, C++ implementations of operators, and the "legacy" operators (TH, THC, THNN, THCUNN), which are legacy, C implementations. The legacy operators are the bad part of town; try not to spend too much time there if you can.
- c10/, which is a pun on Caffe2 and A"Ten" (get it? Caffe 10) contains the core abstractions of PyTorch, including the actual implementations of the Tensor and Storage data structures.

# Reference
- https://blog.ezyang.com/2019/05/pytorch-internals/
- https://pytorch.org/
