# Eileen Chang
# Project 5: Recognition Using Deep Networks
# Task 1F - Read network and run it on test set

# IMPORT STATEMENTS
from main import *

# initialize new set of network and optimizers
continued_network = Net()
continued_optimizer = optim.SGD(network.parameters(), lr=learning_rate,
                                momentum=momentum)

# load the internal state of the network and optimizer when we last saved them
network_state_dict = torch.load("/Users/eileenchang/computervision/project5/results/model.pth")
continued_network.load_state_dict(network_state_dict)

optimizer_state_dict = torch.load("/Users/eileenchang/computervision/project5/results/optimizer.pth")
continued_optimizer.load_state_dict(optimizer_state_dict)

# FUNCTION DEFINITIONS
# read network and run it on test set example_data
def test_set():
    # run test set on the trained network
    with torch.no_grad():
        output = continued_network(example_data)

    # plot the predictions
    fig = plt.figure()
    for i in range(9):
        plt.subplot(3,3,i+1)
        plt.tight_layout()
        plt.imshow(example_data[i][0], cmap='gray', interpolation='none')
        plt.title("Prediction: {}".format(
        output.data.max(1, keepdim=True)[1][i].item()))
        plt.xticks([])
        plt.yticks([])
    fig
    plt.show()

    # print the output prediction values and the correct values to command line
    for i in range(10):
        print("Example {}".format(i+1))
        print("Output Value: {}".format(
        output.data.max(1, keepdim=True)[1][i].item()))
        print("Correct Value: {}".format(
        example_targets[i]))


# MAIN FUNCTION
def main(argv):

    # read network and run it on test set example_data
    test_set()
    
    return

if __name__ == "__main__":
    main(sys.argv)