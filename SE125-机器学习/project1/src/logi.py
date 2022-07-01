import numpy as np
import time
from matplotlib import pyplot as plt
import utils

LEARNING_RATE = 0.021
BATCH_SIZE = 55
MAX_ROUND = 95


def test(weight, input, output, type_data):
    y_pred = np.zeros(input.shape[0])
    for i in range(0, input.shape[0]):
        a = np.dot(weight, input[i])
        y = utils.sigmoid(a)
        y_pred[i] = 1 if y > 0.5 else 0

    # report result
    total = input.shape[0]
    mis = (output != y_pred).sum()
    print("%s  total: %d  mis: %d" % (type_data, total, mis))
    print("Percentage of accuracy: %.2f%%" % ((total - mis) / total * 100))


def train():
    start = time.time()

    # load data
    X_train = utils.read_n("data/X_train.csv", 1)
    y_train = utils.read_n("data/Y_train.csv", 0)
    X_test = utils.read_n("data/X_test.csv", 1)
    y_test = utils.read_n("data/Y_test.csv", 0)
    labels = X_test.shape[1]

    # create initial weight
    weight = np.random.random(labels) - 0.5

    # train
    for k in range(0, MAX_ROUND):
        for m in range(0, X_train.shape[0], BATCH_SIZE):  # foreach sample
            batch_size = min(X_train.shape[0] - m, BATCH_SIZE)
            x_batch = np.empty((batch_size, labels))
            x_batch[0: BATCH_SIZE] = X_train[m: m + BATCH_SIZE]
            a = np.matmul(x_batch, weight.T)
            y = utils.sigmoid(a)
            weight += LEARNING_RATE * np.matmul(x_batch.T, y_train[m: m + batch_size] - y)
    end = time.time()
    print('Time for training: %.2lfs' % (end - start))

    test(weight, X_train, y_train, "TRAINING")
    test(weight, X_test, y_test, "TEST")


def graph():
    # load data
    X_train = utils.read_n("data/X_train.csv", 1)
    y_train = utils.read_n("data/Y_train.csv", 0)
    X_test = utils.read_n("data/X_test.csv", 1)
    y_test = utils.read_n("data/Y_test.csv", 0)
    labels = X_test.shape[1]

    #  create initial weight
    weight = np.random.random(labels) - 0.5
    label_weight = np.zeros(labels)

    lose = mis = 0
    batch_record = np.array([0])
    accuracy_record = np.array([0])
    lose_record = np.array([0])

    # train
    for k in range(0, MAX_ROUND):
        for i in range(0, X_train.shape[0]):
            a = np.dot(weight, X_train[i])
            y = utils.sigmoid(a)
            label_weight += (y_train[i] - y) * X_train[i]
            lose -= y_train[i] * np.log2(y) + (1 - y_train[i]) * np.log2(1 - y)
            mis += (1 if y > 0.5 else 0) != y_train[i]
            if i % BATCH_SIZE == 0 or i == X_train.shape[0] - 1:
                weight += LEARNING_RATE * label_weight
                label_weight = np.zeros(labels)
        batch_record = np.append(batch_record, k)
        accuracy_record = np.append(accuracy_record, (X_train.shape[0] - mis) / X_train.shape[0] * 100)
        lose_record = np.append(lose_record, lose / X_train.shape[0])
        lose = mis = 0

    plt.title("Change of Accuracy During Training")
    plt.xlabel("Round")
    plt.ylabel("Accuracy(%)")
    plt.xlim(0, MAX_ROUND)
    plt.ylim(84, 85)
    plt.plot(batch_record, accuracy_record)
    plt.show()

    plt.title("Change of Loss during Training")
    plt.xlabel("Round")
    plt.ylabel("Loss")
    plt.xlim(0, MAX_ROUND)
    plt.ylim(0.45, 0.5)
    plt.plot(batch_record, lose_record)
    plt.show()


if __name__ == '__main__':
    graph()
    for i in range(0, 4):
        print('Training %d started ----------------------' % i)
        train()
