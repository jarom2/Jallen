import tensorflow as tf
import numpy as np
from tensorflow.python.framework import graph_util
from tensorflow.python.framework import graph_io

tf.reset_default_graph()

x = tf.placeholder(tf.float32, shape=(None, 28, 28), name='x_input')
y = tf.placeholder(tf.float32, shape=(None, 4), name='y_label')
y = tf.stop_gradient(y, name="stop_gradient_y")

input_layer = tf.reshape(x, [-1, 28, 28, 1], name='x_reshaped')
#conv1 = tf.layers.conv2d(
#        inputs=input_layer,
#        filters=32,
#        kernel_size=[5, 5],
#        padding="same",
#        activation=tf.nn.relu,
#        name='conv_1')

#pool1 = tf.layers.max_pooling2d(inputs=conv1, pool_size=[2, 2], strides=2, name='pool_1')

#conv2 = tf.layers.conv2d(
#        inputs=pool1,
#        filters=64,
#        kernel_size=[5, 5],
#        padding="same",
#        activation=tf.nn.relu,
#        name='conv_2')

#pool2 = tf.layers.max_pooling2d(inputs=conv2, pool_size=[2, 2], strides=2, name='pool_2')

#pool2_flat = tf.reshape(pool2, [-1, 7 * 7 * 64], name="pool_2_flat")

fc_layer1 = tf.layers.dense(
        inputs=input_layer, units=64, activation=tf.nn.relu, name='fc_layer_1')

fc_layer2 = tf.layers.dense(
        inputs=fc_layer1, units=64, activation=tf.nn.relu, name='fc_layer_2')

fc_layer3 = tf.layers.dense(
        inputs=fc_layer2, units=64, activation=tf.nn.relu, name='fc_layer_3')

logits = tf.layers.dense(inputs=fc_layer2, units=1, name='logits')
classes = tf.argmax(input=logits, axis=1, name='classes')
probabilities = tf.nn.softmax(logits, name="probabilities_out")
loss = tf.nn.softmax_cross_entropy_with_logits(labels=y, logits=logits, name='loss_func')
grad = tf.gradients(loss, x)
grad_out = tf.identity(grad, name='gradient_out')

optimizer = tf.train.AdamOptimizer()
train_op = optimizer.minimize(loss)

correct = tf.nn.in_top_k(logits, tf.argmax(y, axis=1), 1)
accuracy = tf.reduce_mean(tf.cast(correct, tf.float32))
## GATHER TRAINING DATA ##

x_adv_4 = np.load("/home/jallen/ARFramework_new/output/4xFc.npy")
x_adv_4 = x_adv_4.reshape((6000, 28, 28))

x_adv_train_4 = x_adv_4[0:5499]
x_adv_test_4 = x_adv_4[5500:6000]

x_adv_10 = np.load("/home/jallen/ARFramework_new/output/10xFc.npy")
x_adv_10 = x_adv_10.reshape((6000, 28, 28))  

x_adv_train_10 = x_adv_10[0:5499]
x_adv_test_10 = x_adv_10[5500:6000] 

x_adv_32 = np.load("/home/jallen/ARFramework_new/output/32xFc.npy")
x_adv_32 = x_adv_32.reshape((6000, 28, 28))  

x_adv_train_32 = x_adv_32[0:5499]
x_adv_test_32 = x_adv_32[5500:6000]

x_adv_C = np.load("/home/jallen/ARFramework_new/output/CNA2.npy")
x_adv_C = x_adv_C.reshape((6000, 28, 28))  

x_adv_train_C = x_adv_C[0:5499]
x_adv_test_C = x_adv_C[5500:6000] 

y_adv_train_4 = np.full(5500, 0)
y_adv_train_10 = np.full(5500, 1)
y_adv_train_32 = np.full(5500, 2)
y_adv_train_C = np.full(5500, 3)

y_adv_test_4 = np.full(500, 0)
y_adv_test_10 = np.full(500, 1)
y_adv_test_32 = np.full(500, 2)
y_adv_test_C = np.full(500, 3) 

x_train = np.append(x_adv_train_4, x_adv_train_10, axis=0)
x_train = np.append(x_train, x_adv_train_32, axis=0)
x_train = np.append(x_train, x_adv_train_C, axis=0)

x_test = np.append(x_adv_test_4, x_adv_test_10, axis=0)
x_test = np.append(x_test, x_adv_test_32, axis=0)
x_test = np.append(x_test, x_adv_test_C, axis=0)

y_train = np.append(y_adv_train_4, y_adv_train_10, axis=0)
y_train = np.append(y_train, y_adv_train_32, axis=0)
y_train = np.append(y_train, y_adv_train_C, axis=0)

y_test = np.append(y_adv_test_4, y_adv_test_10, axis=0)
y_test = np.append(y_test, y_adv_test_32, axis=0)
y_test = np.append(y_test, y_adv_test_C, axis=0)

np.save("x_train.npy", x_train)
np.save("y_train.npy", y_train)
np.save("x_test.npy", x_test)
np.save("y_test.npy", y_test)

#x_data = np.load("x_data.npy")
#x_data = x_data.reshape((24000, 28, 28))

#y_data = np.load("y_data.npy")
#y_data = y_data.reshape((24000, 28, 28))

x_train = x_train/np.float32(255)
y_train = y_train.astype(np.int32)
x_test = x_test/np.float32(255)
y_test = y_test.astype(np.int32)

y_train = tf.keras.utils.to_categorical(y_train, 4)
y_test = tf.keras.utils.to_categorical(y_test, 4)
num_epochs = 100
batch_size = 100

init = tf.global_variables_initializer()

with tf.Session() as sess:
    init.run()
    for epoch in range(num_epochs):
        print('Epoch: {}'.format(epoch))
        for i in range(x_train.shape[0] // batch_size):
            batch_indices = np.random.randint(x_train.shape[0], size=batch_size)
            x_batch = x_train[batch_indices]
            y_batch = y_train[batch_indices]
            sess.run(train_op, feed_dict={x: x_batch, y: y_batch})
        acc_test = accuracy.eval(feed_dict={x: x_test, y: y_test})
        print(epoch, "Test accuracy:", acc_test)

    constant_graph = graph_util.convert_variables_to_constants(
            sess, 
            sess.graph.as_graph_def(), 
            ['probabilities_out', 'gradient_out'])

    graph_io.write_graph(constant_graph, '.', 'advEx_gradient_model.pb', as_text=False)

