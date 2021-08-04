from numpy.core.shape_base import block
from pandas.core.frame import DataFrame
from sklearn import metrics
from sklearn import tree
import pandas as pd
from sklearn.model_selection import train_test_split
import matplotlib.pyplot as plt

col_names = ['created_at', 'Humidity', 'Room Temp', 'Fan On']

pima = pd.read_csv("FanOnTraining.csv", header=None, names=col_names)
print(pima.head())


feature_cols = ['Humidity', 'Room Temp']
X = pima[feature_cols]
y = pima['Fan On']

X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.3, random_state=0)

clf = tree.DecisionTreeClassifier(criterion="entropy", max_depth=3)
clf = clf.fit(X_train, y_train)
y_pred = clf.predict(X_test)

print("Accuracy:", metrics.accuracy_score(y_test, y_pred))

# a = plt.axes()
# plt.scatter(X_test['Humidity'], y_pred)
# plt.xlabel('Humidity')
# plt.ylabel('Predictions [Fan On]')
# Xlims = [0, 100]
# Ylims = [-0.5, 1.5]
# plt.xlim(Xlims)
# plt.ylim(Ylims)
# _ = plt.plot(Xlims, Ylims)
# plt.show(block=False)

# a = plt.axes()
# plt.scatter(X_test['Humidity'], y_test)
# plt.xlabel('Humidity')
# plt.ylabel('Actual [Fan On]')
# Xlims = [0, 100]
# Ylims = [-0.5, 1.5]
# plt.xlim(Xlims)
# plt.ylim(Ylims)
# _ = plt.plot(Xlims, Ylims)
# plt.show()
error = DataFrame()
error["Fan On"] = abs(y_pred - y_test)
print(error)
error["Fan On"] = error["Fan On"].astype(bool)
print(error.value_counts())
print(error)
plt.bar(['True', 'False'], error.value_counts())
plt.xlabel('Did the tree guess correctly')
_ = plt.ylabel('Count')
plt.show()
