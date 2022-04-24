# Eileen Chang
# Project 5: Recognition Using Deep Networks
# Task 1G - Test the network on new inputs

# IMPORT STATEMENTS
from cv2 import transform
from matplotlib.colors import Normalize
from main import *
import torch
import numpy as np
from torchvision import transforms, datasets

# initialize new set of network and optimizers
continued_network2 = Net()
continued_optimizer2 = optim.SGD(network.parameters(), lr=learning_rate,
                                momentum=momentum)

# load the internal state of the network and optimizer when we last saved them
network_state_dict = torch.load("/Users/eileenchang/computervision/project5/results/model.pth")
continued_network2.load_state_dict(network_state_dict)

optimizer_state_dict = torch.load("/Users/eileenchang/computervision/project5/results/optimizer.pth")
continued_optimizer2.load_state_dict(optimizer_state_dict)

# FUNCTION DEFINITIONS
# test network on new input 
def handwritten_test(root_path):
    data_transform = transforms.Compose([
            transforms.Resize([28,28]),
            transforms.Grayscale(num_output_channels=1),
            transforms.transforms.ToTensor(),
            transforms.ConvertImageDtype(torch.float),
            transforms.Normalize([0.485], [0.229])
            ])

    my_dataset = datasets.ImageFolder(root=root_path,
            transform=data_transform)

    dataset_loader = torch.utils.data.DataLoader(my_dataset,
            batch_size=batch_size_test, shuffle=True, num_workers=0)

    samples = enumerate(dataset_loader)
    batch_xyz, (sample_data, sample_targets) = next(samples)

    # run the handwritten digit data against the trained network
    with torch.no_grad():
        my_output = continued_network2(sample_data)

    # plot the predictions
    fig = plt.figure()
    for i in range(10):
        plt.subplot(4,3,i+1)
        plt.tight_layout()
        plt.imshow(sample_data[i][0], cmap='gray', interpolation='none')
        plt.title("Prediction: {}".format(
        my_output.data.max(1, keepdim=True)[1][i].item()))
        plt.xticks([])
        plt.yticks([])
    fig
    plt.show()


# MAIN FUNCTION
def main(argv):
    
    # test network on new input 
    handwritten_test("./root/")
    
    return

if __name__ == "__main__":
    main(sys.argv)