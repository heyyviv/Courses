import requests
import sqlite3
import json
from bs4 import BeautifulSoup
import pandas as pd
import random
from collections import Counter
import os
import time

start_time = time.time()

def createDatabaseConnect(dbName):
        con = sqlite3.connect(dbName)
        cur = con.cursor()
        return cur,con

def most_common(lst):
    return max(set(lst), key=lst.count)

while True:
    dbName = "OlympicsData.db"
    cursor,con = createDatabaseConnect(dbName)
    query = "SELECT DoneORNotDone from Olympics"
    result = cursor.execute(query)
    flag=1
    for row in result:
        #print(row)
        if(row[0]=='0'):
            flag=0
    
    if flag==1:
        break
    cursor.close()
    time.sleep(2)

dbName = "OlympicsData.db"
cursor,con = createDatabaseConnect(dbName)
query = "SELECT Year from Olympics"
result = cursor.execute(query)
print("Year choosed")
for row in result:
    print(row)

query = "SELECT R1 FROM Olympics"                                                                                                       
result = cursor.execute(query)
rank_list=[]
for row in result:
    rank_list.append(row[0])

query = "SELECT R2 FROM Olympics"                                                                                                       
result = cursor.execute(query)
for row in result:
    rank_list.append(row[0])

query = "SELECT R3 FROM Olympics"                                                                                                       
result = cursor.execute(query)
for row in result:
    rank_list.append(row[0])
rank_list=list(filter(lambda a: a != "None", rank_list))
print("Most common Country in Rank List")
print(most_common(rank_list))


query="SELECT Avg(Athletes) FROM Olympics"
result = cursor.execute(query)
print("average Atlete")
print(result.fetchone())

cursor.close()
print("My program took" + str( time.time() - start_time)+"to run")


