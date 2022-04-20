# Eileen Chang
# Project 5: Recognition Using Deep Networks

# IMPORT STATEMENTS
import sys
import numpy
import random
import torch
import torchvision
import matplotlib.pyplot as plt
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim

# PREPARE DATASET
n_epochs = 3
batch_size_train = 64
batch_size_test = 1000
learning_rate = 0.01
momentum = 0.5
log_interval = 10

random_seed = 1
torch.backends.cudnn.enabled = False
torch.manual_seed(random_seed)

# DATA LOADER
train_loader = torch.utils.data.DataLoader(
  torchvision.datasets.MNIST('/Users/eileenchang/computervision/project5/images', train=True, download=True,
                             transform=torchvision.transforms.Compose([
                               torchvision.transforms.ToTensor(),
                               torchvision.transforms.Normalize(
                                 (0.1307,), (0.3081,))
                             ])),
  batch_size=batch_size_train, shuffle=True)

test_loader = torch.utils.data.DataLoader(
  torchvision.datasets.MNIST('/Users/eileenchang/computervision/project5/images', train=False, download=True,
                             transform=torchvision.transforms.Compose([
                               torchvision.transforms.ToTensor(),
                               torchvision.transforms.Normalize(
                                 (0.1307,), (0.3081,))
                             ])),
  batch_size=batch_size_test, shuffle=True)


# FUNCTIONS 

# first 6 example digits
def example():
  examples = enumerate(test_loader)
  batch_idx, (example_data, example_targets) = next(examples)

  example_data.shape
  torch.Size([1000, 1, 28, 28])

  # plot first 6 example digits
  fig = plt.figure()
  for i in range(6):
    plt.subplot(2,3,i+1)
    plt.tight_layout()
    plt.imshow(example_data[i][0], cmap='gray', interpolation='none')
    plt.title("Ground Truth: {}".format(example_targets[i]))
    plt.xticks([])
    plt.yticks([])
  fig
  plt.show()


# MAIN FUNCTION
def main(argv):
    # handle any command line arguments in argv

    # main function code

    # in order to make network repeatable set the random seed for the torch package
    torch.manual_seed(42) # remove this line if you want to create different networks
    torch.backends.cudnn.enabled = False # turn off CUDA

    example()

    return

if __name__ == "__main__":
    main(sys.argv)
