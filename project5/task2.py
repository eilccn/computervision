# Eileen Chang
# Project 5: Recognition Using Deep Networks
# Task 2 - Examine the network

# IMPORT STATEMENTS
from cv2 import filter2D, transform
from matplotlib.colors import Normalize
from main import *
import torch
import numpy as np
from torchvision import transforms, datasets
from torchvision import models
from torchinfo import summary
import cv2

# READ NETWORK
# initialize new set of network and optimizers
continued_network3 = Net()
continued_optimizer3 = optim.SGD(network.parameters(), lr=learning_rate,
                                momentum=momentum)

device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
model = Net().to(device)

# load the internal state of the network and optimizer when we last saved them
def load_network():
    network_state_dict = torch.load("/Users/eileenchang/computervision/project5/results/model.pth")
    continued_network3.load_state_dict(network_state_dict)

    optimizer_state_dict = torch.load("/Users/eileenchang/computervision/project5/results/optimizer.pth")
    continued_optimizer3.load_state_dict(optimizer_state_dict)

# print model
def print_model():
    #summary(model,(1,28,28))
    summary(model, input_size=(batch_size_test, 1, 28, 28))

# FUNCTION DEFINITIONS
# TASK 1A: get, print, and plot the weights of the first layer, conv1
def firstlayer_weights():
        # get weights
        with torch.no_grad():
            weights = model.conv1.weight

        # print weights with shape [10,1,5,5]
        print(weights)

        # normalize filter values to 0-1 so we can visualize them
        f_min, f_max = weights.min(), weights.max()
        filters = (weights - f_min) / (f_max - f_min)
        filter_cnt=1

        # visualize/plot the filters
        idx = 1
        fig = plt.figure()
        for i in range(10):
            plt.subplot(3,4,i+1)
            plt.tight_layout()
            with torch.no_grad():
                plt.imshow(weights[i][0], interpolation='none')
            plt.title("Filter: {}".format(idx))
            plt.xticks([])
            plt.yticks([])
            idx+=1
        fig
        plt.show()

# TASK 2B: show the effect of the filters
def show_effect():
    with torch.no_grad():
        # get weights
        weights = model.conv1.weight

        #get train example
        global example_data
        example_data = example_data[0][0].numpy().astype(np.float32)

        # visualize/plot the filters
        idx = 1
        fig = plt.figure()
        for i in range(10):
            plt.subplot(5,2,i+1)
            plt.tight_layout()
            with torch.no_grad():
                filters = weights[i][0].numpy().astype(np.float32)
                filtered_outputs = cv2.filter2D(example_data, -1, filters)
                plt.imshow(filtered_outputs, cmap='gray', interpolation='none')
            plt.title("Filter: {}".format(idx))
            plt.xticks([])
            plt.yticks([])
            idx+=1
        fig
        plt.show()




# MAIN FUNCTION
def main(argv):

    # read and load the internal state of the network and optimizer when we last saved them
    load_network()

    # print model
    print_model()

    # TASK 1A: get, print, and plot the weights of the first layer, conv1
    firstlayer_weights()

    # TASK 2B: show effect of the filters
    show_effect()
    
    return

if __name__ == "__main__":
    main(sys.argv)
