import requests
import sqlite3
import json
from bs4 import BeautifulSoup

def getData(url):
    response = requests.get(url)
    #convert to text string and return 
    return response.text

def convertJson(data):
    return json.loads(data)
    
def createDatabaseConnect(dbName):
	con = sqlite3.connect(dbName)
	cur = con.cursor()
	return cur,con
# We will call https://cse.iitkgp.ac.in/~mainack/computing-lab/web-scraping/json.php
# Lets see what it returns
for i in range(4):
	lat=22.5726+2*i
	lon=88.3639+2.5*i
	url = f'https://api.openweathermap.org/data/2.5/weather?lat={lat}&lon={lon}&appid=9b26f315bfd87c1a685b5b0622f46c50'
	returnedData = getData(url)
	print(returnedData)
	## it will print {"a":"Apple","b":"Ball","c":"Cat"}

	## This is structured data which can be converted into json format. 
	## lets do that 
	jsonData = convertJson(returnedData)
	## Now jsonData contains a python dict
	print(jsonData["name"])
	## will print "Apple"



	dbName = "Weather.db"
	cursor,con = createDatabaseConnect(dbName)

	## Now you can create Table and insert/select records from there
	## Lets create a Table "example" with three columns a, b and c to insert the structured data 
	## we fecthed earlier

	query = "CREATE TABLE IF NOT EXISTS city_weather(City, Tempreture, Description,Humidity,WindSpeed)"
	cursor.execute(query)

	query = "INSERT INTO city_weather VALUES ('%s', '%s', '%s','%s','%s')"%(jsonData["name"], jsonData["main"]["temp"], jsonData["weather"][0]["description"],jsonData["main"]["humidity"],jsonData["wind"]["speed"])
	cursor.execute(query)

	## Lets see what is in the table
	query = "SELECT * from city_weather"
	result = cursor.execute(query)
	con.commit()
	for row in result:
		print(row)
	cursor.close()

