# Eileen Chang
# Project 5: Recognition Using Deep Networks
# Task 1F - Read network and run it on test set

# IMPORT STATEMENTS
from main import *


with torch.no_grad():
    output = network(example_data)

fig = plt.figure()
for i in range(9):
  plt.subplot(2,3,i+1)
  plt.tight_layout()
  plt.imshow(example_data[i][0], cmap='gray', interpolation='none')
  plt.title("Prediction: {}".format(
    output.data.max(1, keepdim=True)[1][i].item()))
  plt.xticks([])
  plt.yticks([])
fig


