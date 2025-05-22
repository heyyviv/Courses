---
layout: post
title: "Data analytics part 1"
date: 2024-08-27 01:43:18 +0530
categories: Deep Learning
---

Data comes from many sources: sensor measurements, events, text, images, and videos.
Clickstreams are sequences of actions by a user interacting with an app or web page

A sample is a subset of data from a larger data set; statisticians call this larger data set the population
Sample A subset from a larger data set.
Population The larger data set or idea of a data set.
N (n) The size of the population (sample).
Random sampling Drawing elements into a sample at random.
Stratified sampling Dividing the population into strata and randomly sampling from each strata.
Simple random sample -> The sample that results from random sampling without stratifying the population.
Sample bias -> A sample that misrepresents the population.

Random sampling is a process in which each available member of the population being sampled has an equal chance of being chosen for the sample at each draw.
The sample that results is called a simple random sample. 
Sampling can be done with replacement, in which observations are put back in the population after each
draw for possible future reselection. Or it can be done without replacement, in which case observations, once selected, are unavailable for future draws.


Example The reviews of restaurants, hotels, cafes, and so on that you read on social media sites like Yelp
are prone to bias because the people submitting them are not randomly selected; rather, they
themselves have taken the initiative to write. This leads to self-selection bias — the people
motivated to write reviews may be those who had poor experiences, may have an association
with the establishment, or may simply be a different type of person from those who do not write
reviews. Note that while self-selection samples can be unreliable indicators of the true state of
affairs, they may be more reliable in simply comparing one establishment to a similar one; the
same self-selection bias might apply to each

Statistical bias refers to measurement or sampling errors that are systematic and produced by the measurement or sampling process. 
Random sampling is not always easy. Proper definition of an accessible population is key.
Suppose we want to generate a representative profile of
customers and we need to conduct a pilot customer survey. The survey needs to be
representative but is labor intensive.
First we need to define who a customer is. We might select all customer records
where purchase amount > 0. Do we include all past customers? Do we include
refunds? Internal test purchases? Resellers? Both billing agent and customer?
Next we need to specify a sampling procedure. It might be “select 100 customers
at random.” Where a sampling from a flow is involved (e.g., real-time customer
transactions or web visitors), timing considerations may be important (e.g., a web
visitor at 10 a.m. on a weekday may be different from a web visitor at 10 p.m. on
a weekend).

In stratified sampling, the population is divided up into strata, and random
samples are taken from each stratum. Political pollsters might seek to learn the
electoral preferences of whites, blacks, and Hispanics.A simple random sample taken from the population would yield too few blacks and Hispanics, so those strata could be overweighted in stratified sampling to yield equivalent sample
sizes

 Time and effort spent on random sampling not only reduce bias, but also allow greater
attention to data exploration and data quality.For example, missing data and
outliers may contain useful information. 

So when are massive amounts of data needed?
The classic scenario for the value of big data is when the data is not only big, but
sparse as well. Consider the search queries received by Google, where columns
are terms, rows are individual search queries, and cell values are either 0 or 1,
depending on whether a query contains a term. The goal is to determine the best
predicted search destination for a given query. There are over 150,000 words in
the English language, and Google processes over 1 trillion queries per year. This
yields a huge matrix, the vast majority of whose entries are "0"
This is a true big data problem — only when such enormous quantities of data are
accumulated can effective search results be returned for most queries. And the
more data accumulates, the better the results.

Data quality is often more important than data quantity, and random sampling can reduce bias and
facilitate quality improvement that would be prohibitively expensive.
The term sampling distribution of a statistic refers to the distribution of some
sample statistic, over many samples drawn from the same population

# Sampling distribution *

Sample statistic -> A metric calculated for a sample of data drawn from a larger population.
Data distribution -> The frequency distribution of individual values in a data set.
Sampling distribution -> The frequency distribution of a sample statistic over many samples or resamples.
Central limit theorem -> The tendency of the sampling distribution to take on a normal shape as sample size rises.
Standard error -> The variability (standard deviation) of a sample statistic over many samples (not to be confused with standard deviation, which, by itself, refers to variability of individual data values)

The distribution of a sample statistic such as the mean is likely to be more regular
and bell-shaped than the distribution of the data itself. The larger the sample that
the statistic is based on, the more this is true. Also, the larger the sample, the
narrower the distribution of the sample statistic.
Theorem on Sampling distribution 

# Central Limit Theorem * 
It says that the means drawn
from multiple samples will resemble the familiar bell-shaped normal curve (see “Normal Distribution”), even if the source population is not normally distributed, provided that the sample size is large enough and the departure of the data from normality is not too great

If original distribution of data from sample are taken have mean -> u and variance-> sigma
then sample(sample size = n) mean -> x will have a normal distribution of mean -> u and variance sigma/n.

The practice of studying random phenomena shows that the results of individual observations, even those made under the same conditions, may differ. But the average results for a sufficiently large number of observations are stable and only slightly fluctuate depending on the results of individual observations. The theoretical basis for this remarkable property of random phenomena is the Central Limit Theorem(aka law of large numbers).

The average value of the data sample, according to the Central Limit Theorem, will be closer to the average of the whole population and will be approximately normal as the sample size increases. The significance of this theorem follows from the fact that this is true regardless of population distribution.

# Standard error
The standard error is a single metric that sums up the variability in the sampling
distribution for a statistic. The standard error can be estimated using a statistic
based on the standard deviation s of the sample values, and the sample size n:

Standard error = S/root(n)

As the sample size increases, the standard error decreases, corresponding to what
was observed in Figure 2-6. The relationship between standard error and sample
size is sometimes referred to as the square-root of n rule: in order to reduce the
standard error by a factor of 2, the sample size must be increased by a factor of 4.

# The Bootstrap
One easy and effective way to estimate the sampling distribution of a statistic, or
of model parameters, is to draw additional samples, with replacement, from the
sample itself and recalculate the statistic or model for each resample. This
procedure is called the bootstrap, and it does not necessarily involve any
assumptions about the data or the sample statistic being normally distributed

Bootstrap sample -> A sample taken with replacement from an observed data set.
Resampling -> The process of taking repeated samples from observed data; includes both bootstrap and
permutation (shuffling) procedures.

Conceptually, you can imagine the bootstrap as replicating the original sample
thousands or millions of times so that you have a hypothetical population that
embodies all the knowledge from your original sample (it’s just larger). You can
then draw samples from this hypothetical population for the purpose of estimating
a sampling distribution.

![Boostrap](/assets/da_s1.jpg)

# Chi-Square Distribution
A chi-square (Χ2) distribution is a continuous probability distribution that is used in many hypothesis tests.
The shape of a chi-square distribution is determined by the parameter k. The graph below shows examples of chi-square distributions with different values of k.
![DA](/assets/da_s2.jpg)

The shape of a chi-square distribution is determined by the parameter k, which represents the degrees of freedom.
The main purpose of chi-square distributions is hypothesis testing, not describing real-world distributions.

Imagine taking a random sample of a standard normal distribution (Z). If you squared all the values in the sample, you would have the chi-square distribution with k = 1.

Χ21 = (Z)2

Now imagine taking samples from two standard normal distributions (Z1 and Z2). If each time you sampled a pair of values, you squared them and added them together, you would have the chi-square distribution with k = 2.

Χ22 = (Z1)2 + (Z2)2

More generally, if you sample from k independent standard normal distributions and then square and sum the values, you’ll produce a chi-square distribution with k degrees of freedom. 

Χ2k = (Z1)2 + (Z2)2 + … + (Zk)2

# Hypothesis Tests
When we refer to Hypothesis Testing, it means using a systematic procedure to decide whether data and research study can support our particular theory which applies to a population. 

We do this by using two mutually exclusive hypotheses about a population, and evaluating these statements to decide if the statements are supported by the sample data.

When performing Hypothesis Testing, firstly, a hypothesis must be formulated. An example of a hypothesis is “there is a correlation between height and gender in a population,” or “there is a difference between two groups of a population.”

Usually, the thesis to be demonstrated is called the Alternative Hypothesis (HA), and its opposite is the Null Hypothesis (H0). In practice, the Null Hypothesis states that there is nothing new happening in the population.

The objective of Hypothesis Testing is to verify if the Null Hypothesis can be rejected or not. In general, rejecting the Null Hypothesis does not automatically mean that the alternative hypothesis is accepted. However, in some cases, rejecting the Null Hypothesis can imply that the Alternative Hypothesis can be accepted.

When performing a Hypothesis Test, two types of errors could occur:
* Type-I Error: reject the Null Hypothesis when it is actually true.
* Type-II Error: accept the Null Hypothesis when it is actually false.

Null Hypothesis
There is no relation between statistical variables and refer to this type of testing as null hypothesis testing. A null hypothesis is represented as H0. 

 
Alternative Hypothesis
There is a relationship between two variables, proving that they have a statistical bond. An alternative hypothesis is represented as H1 or HA. The alternative hypothesis can be split into:

One-tailed. This is when you are testing in one direction and disregarding the possibility of a relationship with another variable in another direction. The sample mean would be higher or lower than the population mean, but not both.
Two-tailed. This is when you are testing in both directions and shows whether the sample mean is higher than or less than the mean of a population.

Types of Hypothesis Testing
- Parametric tests  are used when the samples have a normal distribution. In general, samples with a mean of 0 and a variance of 1 follow a normal distribution
- Non-Parametric tests  If the samples do not follow a normal distribution, non-parametric tests are used.

Two types of Hypothesis Testing can be created depending on the number of samples to be compared:
- One Sample  If there is only one sample that must be compared to a specific value, it is called a single sample.
- Two Samples  if you're comparing two or more samples. Correlation and sample difference are two tests that could be used in this situation. Samples can be paired or not in both circumstances. Dependent samples are sometimes known as paired samples, while independent samples are known as unpaired samples. Natural or matched couplings occur in paired samples.






