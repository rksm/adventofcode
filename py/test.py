import time

#print(time.mktime([0,30]))

print(time.strptime("00:30", "%H:%M"))
t = time.strptime("01:30", "%H:%M")
t

secs_in_day = 24*60*60
print(secs_in_day)
print(30*60)
h = 2
m = 30
sec_count = h * 60*60 + m * 60
start_min = sec_count / 60
end_min = secs_in_day / 60
remaining_min = (secs_in_day - sec_count) /60

print(start_min)
print(end_min)
print(remaining_min)
print(end_min - start_min)

