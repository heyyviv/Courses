import requests
import sqlite3
import json
from bs4 import BeautifulSoup
import pandas as pd
import random
from collections import Counter

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
    return rank1,rank2,rank3,country_list

    table =soup.select_one('table:-soup-contains("Events at the")')
    
    table_row=table.find("tbody")
    table_row=table_row.find_all("tr")
    
    l_names=table_row[1].find_all("li")
    sports_names=[]
    for row in l_names:
        sports_names.append(row.get_text())
    '''
    tables=soup.find_all('table')
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
        
    
    #print(sports_names)
    return rank1,rank2,rank3,country_list,sports_names


def scrap(text):
    #print(text)
    url_temp=text.find_all("a")
    #print("#############")
    #print(url_temp[1].get('href'))
    url="https://en.wikipedia.org"+url_temp[1].get('href')
    title=url_temp[1].get('title')
    year=text.find("td").get_text()[0:4]
    #print(url)
    print(title)
    #print(year)
    #need host city
    t_data=text.find_all("td")
    host = t_data[2].find_all("a")[-1].get_text()
    #print(host)
    #no of athlete
    n_athlete = t_data[5].get_text()
    #print(n_athlete)
    #no of sports
    n_sport = t_data[4].get_text().split(" ")[0]
    #print(n_sport)
    r1,r2,r3,country_list,sport_name=find_ranks(url)
    
    #####database
    dbName = "Olympics.db"
    cursor,con = createDatabaseConnect(dbName)

    ## Now you can create Table and insert/select records from there
    ## Lets create a Table "example" with three columns a, b and c to insert the structured data 
    ## we fecthed earlier

    query = "CREATE TABLE IF NOT EXISTS Olympics(Name, Year, Host,Nations,Athletes,Sports,R1,R2,R3)"
    cursor.execute(query)
    c_list='+'.join(country_list)
    c_sports='+'.join(sport_name)
    query = "INSERT INTO Olympics VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')"%(title, year,host, c_list,n_athlete , c_sports,r1,r2,r3)
    cursor.execute(query)
    con.commit()
    cursor.close()
    

url = 'https://en.wikipedia.org/wiki/Summer_Olympic_Games'
html_doc = getData(url)

#print(html_doc)
## it will print 
## <html>
## ...
## which in unstructured data, cannot be converted into json :( 
## bs4 to the rescue
## see the documentation here: 
soup = BeautifulSoup(html_doc, 'html.parser')

## Now bs4 will do the job for you
## Say we want to find all text within h1 tags
texts = soup.find("table", class_="sortable wikitable")
texts_body= texts.find("tbody")
#print(texts)
texts= texts_body.find_all("tr")
#print(len(texts))


while(True):
    try:
        index1 = random.randint(15,33)
        scrap(texts[index1])
        index2 = random.randint(15,33)
        scrap(texts[index2])
        break
    except:
        dbName = "Olympics.db"
        cursor,con = createDatabaseConnect(dbName)
        cursor.executescript('drop table if exists Olympics;')
        continue


dbName = "Olympics.db"
cursor,con = createDatabaseConnect(dbName)

query = "SELECT Year from Olympics"
result = cursor.execute(query)
for row in result:
    print(row)
query = "SELECT Nations from Olympics"
result = cursor.execute(query)
c=0
for row in result:
    c=c+len(row[0].split('+'))
print("Average nation "+str(c/2))

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
cursor.executescript('drop table if exists Olympics;')

print(list(set([i for i in rank_list if rank_list.count(i) > 1])))

bName = "Olympics.db"
cursor,con = createDatabaseConnect(dbName)
cursor.executescript('drop table if exists Olympics;')
con.commit()

