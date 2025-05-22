---
layout: post
title: "Recurrent Neural Networks"
date: 2024-08-26 01:43:18 +0530
categories: Deep Learning
---

To go from multilayer networks to recurrent networks, we need to take advantageof one of the early ideas found in machine learning and statistical models of the1980s: sharing parameters across diﬀerent parts of a model. Parameter sharingmakes it possible to extend and apply the model to examples of diﬀerent forms(diﬀerent lengths, here) and generalize across them. If we had separate parametersfor each value of the time index, we could not generalize to sequence lengths notseen during training, nor share statistical strength across diﬀerent sequence lengthsand across diﬀerent positions in time. Such sharing is particularly important whena speciﬁc piece of information can occur at multiple positions within the sequence.

“I went to Nepal in 2009” and “In 2009,I went to Nepal.” If we ask a machine learning model to read each sentence andextract the year in which the narrator went to Nepal, we would like it to recognizethe year 2009 as the relevant piece of information, whether it appears in the sixth word or in the second word of the sentence.
A traditional fully connectedfeedforward network would have separate parameters for each input feature, soit would need to learn all the rules of the language separately at each position inthe sentence. By comparison, a recurrent neural network shares the same weightsacross several time steps.

![RNN](/assets/rnn_1.jpg)
![RNN](/assets/rnn_2.jpg)

When the recurrent network is trained to perform a task that requires predictingthe future from the past, the network typically learns to use h(t) as a kind of lossysummary of the task-relevant aspects of the past sequence of inputs up tot. Thissummary is in general necessarily lossy, since it maps an arbitrary length sequence(x(t), x(t−1), x(t−2), . . . , x(2), x(1)) to a ﬁxed length vectorh(t). Depending on the training criterion, this summary might selectively keep some aspects of the pastsequence with more precision than other aspects.
![RNN](/assets/rnn_4.jpg)
![RNN](/assets/rnn_5.jpg)
![RNN](/assets/rnn_6.jpg)
![Seq2Seq](/assets/rnn_3.jpg)

Hidden layers are, as explained, layers that are hidden from view on the path from input to output. Hidden states are technically speaking inputs to whatever we do at a given step, and they can only be computed by looking at data at previous time steps.
Recurrent neural networks (RNNs) are neural networks with hidden states.
![RNN](/assets/rnn_7.jpg)

Character Level RNN vs Word Level RNN
It’s not as good as the word-level RNN at capturing long-distance dependencies. For example, to predict up to the third word from the beginning of a sentence, a word-level RNN has to make two predictions to get there, whereas a character-level RNN has to make predictions for the number of times equal to the number of characters before the second space. The more predictions the RNN has to make, the more error-prone the result is.

Also it’s harder to train. The cross entropy loss takes the sum over all elements in each sentence. For a word-level RNN, the number of elements equals the number of words in the sentence, whereas for a character-level RNN, the number of elements equals the number of characters in the sentence. Thus, in training, it takes a longer path to propagate the error from the softmax at the last time step back to the beginning.

If the attention mechanism is used, then the attention matrix is also much larger in a character-level RNN than in a word-level RNN, just because there are more characters to attend to in the previous sentence than there are words.
![RNN](/assets/rnn_8.jpg)

A particular problem with training deep networks..
The gradient of the error with respect to weights is unstable..
During BackPropogation
![RNN](/assets/rnn_14.jpg)
- In linear systems, long-term behavior depends entirely on the eigenvalues of the recurrent weights matrix– If the largest Eigen value is greater than 1, the system will “blow up” – If it is lesser than 1, the response will “vanish” very quickly– Complex Eigen values cause oscillatory response but with the same overall trends
- Magnitudes greater than 1 will cause the system to blow up
- The rate of blow up or vanishing depends only on the Eigen values and not on the input

LSTM Networks
![RNN](/assets/rnn_9.jpg)
In the above diagram, each line carries an entire vector, from the output of one node to the inputs of others. The pink circles represent pointwise operations, like vector addition, while the yellow boxes are learned neural network layers. Lines merging denote concatenation, while a line forking denote its content being copied and the copies going to different locations.

Step-by-Step LSTM Walk Through
The first step in our LSTM is to decide what information we’re going to throw away from the cell state. This decision is made by a sigmoid layer called the “forget gate layer.” It looks at ht−1 and xt , and outputs a number between 0
 and 1 for each number in the cell state Ct−1 . A 1 represents “completely keep this” while a 0 represents “completely get rid of this.”
Let’s go back to our example of a language model trying to predict the next word based on all the previous ones. In such a problem, the cell state might include the gender of the present subject, so that the correct pronouns can be used. When we see a new subject, we want to forget the gender of the old subject.
![RNN](/assets/rnn_10.jpg)
The next step is to decide what new information we’re going to store in the cell state. This has two parts. First, a sigmoid layer called the “input gate layer” decides which values we’ll update. Next, a tanh layer creates a vector of new candidate values, C~t
, that could be added to the state. In the next step, we’ll combine these two to create an update to the state.

In the example of our language model, we’d want to add the gender of the new subject to the cell state, to replace the old one we’re forgetting.
![RNN](/assets/rnn_11.jpg)
It’s now time to update the old cell state, Ct−1, into the new cell state Ct. The previous steps already decided what to do, we just need to actually do it.We multiply the old state by ft  forgetting the things we decided to forget earlier. Then we add it∗C~t
. This is the new candidate values, scaled by how much we decided to update each state value.

In the case of the language model, this is where we’d actually drop the information about the old subject’s gender and add the new information, as we decided in the previous steps.
![RNN](/assets/rnn_12.jpg)
Finally, we need to decide what we’re going to output. This output will be based on our cell state, but will be a filtered version. First, we run a sigmoid layer which decides what parts of the cell state we’re going to output. Then, we put the cell state through tanh
 (to push the values to be between −1 and 1
) and multiply it by the output of the sigmoid gate, so that we only output the parts we decided to.

For the language model example, since it just saw a subject, it might want to output information relevant to a verb, in case that’s what is coming next. For example, it might output whether the subject is singular or plural, so that we know what form a verb should be conjugated into if that’s what follows next.
![RNN](/assets/rnn_13.jpg)
LSTM doesn’t guarantee that there is no vanishing/exploding gradient, but it does
provide an easier way for the model to learn long-distance dependencies

While quantitative comparisons are useful, they only provide partial insight into the how a recurrent unit memorizes. A model can, for example, achieve high accuracy and cross entropy loss by just providing highly accurate predictions in cases that only require short-term memorization, while being inaccurate at predictions that require long-term memorization. For example, when autocompleting words in a sentence, a model with only short-term understanding could still exhibit high accuracy completing the ends of words once most of the characters are present. However, without longer term contextual understanding it won’t be able to predict words when only a few characters are known.

![RNN](/assets/rnn_15.jpg)