#import csv
#with open("./weather_data.csv") as data_file:
#    data = csv.reader(data_file)
#    
#    temperatures = []
#    
#    for row in data:
#        if row[1] != 'temp':
#            temperatures.append(int(row[1]))
#        
#    print(temperatures)


import pandas


data = pandas.read_csv('weather_data.csv')

print(data)


#print a respective column

print(data['temp'])

# average temperature

temp_list = data['temp'].to_list()
number_of_records = len(temp_list)
total_temp = sum(temp_list)


average_temp = total_temp / number_of_records

print(average_temp)
    

# max temperature
max(temp_list)

print(max(temp_list))

# get data from a row

print(data[data.day == 'Monday'])

# get top tempraure row

print(data[data.temp == data.temp.max()])

monday = data[data.day == 'Monday']

print(monday)

monday_temp = monday.temp[0]
monday_temp_F = monday_temp * 9/5 + 32
print(monday_temp_F)


data_dic = {
    "students" : ["Ammy","James", "Angela"],
    "scores": [76,56,65]
}

data = pandas.DataFrame(data_dic)

print(data)

data.to_csv("new_data.csv")