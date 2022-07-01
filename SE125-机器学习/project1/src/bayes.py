from sklearn.naive_bayes import GaussianNB
from sklearn.naive_bayes import BernoulliNB
from sklearn.naive_bayes import MultinomialNB
import time
import utils

# X_train = utils.read_n("data/X_train.csv", 1)
# y_train = utils.read_n("data/Y_train.csv", 0)
# X_test = utils.read_n("data/X_test.csv", 1)
# y_test = utils.read_n("data/Y_test.csv", 0)
# X_train = utils.read_s("data/X_train.csv", 1)
# y_train = utils.read_s("data/Y_train.csv", 0)
# X_test = utils.read_s("data/X_test.csv", 1)
# y_test = utils.read_s("data/Y_test.csv", 0)


def test_and_report(input, output, type_bayes, type_data):
    start = time.time()

    if type_bayes == 0:
        model = BernoulliNB()
    elif type_bayes == 1:
        model = GaussianNB()
    else:
        model = MultinomialNB()
    y_pred = model.fit(input, output).predict(input)
    total = input.shape[0]
    mis = (output != y_pred).sum()

    end = time.time()
    print('Time for training: %.2lfs' % (end - start))
    print("%s total: %d  mis: %d" % (type_data, total, mis))
    print("Percentage of accuracy: %.2f%%" % ((total - mis) / total * 100))


def main():
    start = time.time()
    # load data
    X_train = utils.read_n("data/X_train.csv", 1)
    y_train = utils.read_n("data/Y_train.csv", 0)
    X_test = utils.read_n("data/X_test.csv", 1)
    y_test = utils.read_n("data/Y_test.csv", 0)
    end = time.time()
    print('Time for load data: %.2lfs' % (end - start))

    print("Using BernoulliNB -------------------")
    test_and_report(X_train, y_train, 0, "TRAINING")
    test_and_report(X_test, y_test, 0, "TEST")
    print("Using GaussianNB --------------------")
    test_and_report(X_train, y_train, 1, "TRAINING")
    test_and_report(X_test, y_test, 1, "TEST")
    print("Using MultinomialNB -----------------")
    test_and_report(X_train, y_train, 2, "TRAINING")
    test_and_report(X_test, y_test, 2, "TEST")


if __name__ == '__main__':
    main()
