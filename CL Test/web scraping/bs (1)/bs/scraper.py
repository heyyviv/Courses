import requests
import sqlite3
import json
from bs4 import BeautifulSoup
import pandas as pd
import random
from collections import Counter
import sys



def getData(url):
        
        
    headers = {'User-Agent': "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/42.0.2311.135 Safari/537.36 Edge/12.246"} 
    response = requests.get(url,headers=headers)
    #convert to text string and return 
    return response.text

def convertJson(data):
    return json.loads(data)

def createDatabaseConnect(dbName):
    con = sqlite3.connect(dbName)
    cur = con.cursor()
    return cur,con

def find_ranks(url):
    html=getData(url)
    soup = BeautifulSoup(html, 'html.parser')
    table =soup.select_one('table:-soup-contains("Participating National Olympic Committees")')    
    table_row=table.find_all("li")
    country_list=[]
    for row in table_row:
        country_list.append(row.get_text().split("(")[0])
    '''
    try:
        table =soup.select_one('table:-soup-contains("Olympics medal table")')  
        table_row=table.find_all("tr")
        rank1=table_row[1].find("a").get_text()
        print(rank1)
        rank2=table_row[2].find("a").get_text()
        rank3=table_row[3].find("a").get_text()
    except:
        rank1=None 
        rank2=None 
        rank3=None 
    return rank1,rank2,rank3,country_list'''
    tables=soup.find_all('table')
    rank1=None 
    rank2=None 
    rank3=None 
    for table in tables:
        try:
            if(table.find("tr").find("th").get_text()=="Rank"):
                table_row=table.find_all("tr")
                rank1=table_row[1].find("a").get_text()
                #print(rank1)
                rank2=table_row[2].find("a").get_text()
                rank3=table_row[3].find("a").get_text()
                break
        except:
            continue
    
    table =soup.select_one('table:-soup-contains("Events at the")')
    
    table_row=table.find("tbody")
    table_row=table_row.find_all("tr")
    
    l_names=table_row[1].find_all("li")
    sports_names=[]
    for row in l_names:
        sports_names.append(row.get_text())
        
    '''s_names=table_row[1].find_all("li")

    print(s_names.get_text()) '''

    return rank1,rank2,rank3,country_list,sports_names

while(True):
    dbName = "OlympicsData.db"
    cursor,con = createDatabaseConnect(dbName)
    query = "SELECT * from Olympics ORDER BY RANDOM()"
    result = cursor.execute(query).fetchall()
    cursor.close()
    flag=0
    for row in result:
            if(row[10]=='0'):
                link=row[1]
                r1,r2,r3,country_list,sport_name=find_ranks(link)
                flag=1
                c_list="@".join(country_list)
                s_list="@".join(sport_name)
                c_list = c_list.replace("'", "")
                #print(link)
                try:
                    cursor,con = createDatabaseConnect(dbName)
                    query="UPDATE Olympics SET Nations='%s',Sports='%s',R1='%s',R2='%s',R3='%s',DoneORNotDone='%s' WHERE URL='%s'"%(c_list,s_list,r1,r2,r3,1,link)
                    cursor.execute(query)
                except:
                    print(c_list)
                    print(s_list)
                    flag=0
                con.commit()
                cursor.close()
                break
    cursor.close()
    if flag==0:
        break

sys.exit()
