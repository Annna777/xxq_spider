import re
from selenium import webdriver
from selenium.webdriver.common.keys import Keys
import time
from threading import Timer
import js
# import pyautogui
driver = webdriver.Chrome()

def func():

    driver.get('http://piyao.sina.cn/')

    time.sleep(1)

    for j in range(0,30):

        driver.execute_script('window.scrollTo(0,document.body.scrollHeight)') #模拟鼠标滚到最低
        j+=1
        #last_review = driver.find_element_by_css_selector('div[style$="sp.gif"]').click()
        #driver.execute_script('arguments[0].scrollIntoView(true);', last_review)
        #js="var q=documentElement.scrollTop=10000"
        #driver.execute_script(js)
        #pyautogui.scroll(1000)
        time.sleep(4)
    data = driver.find_elements_by_xpath('//div[1]/div[1]/div[3]/div[1]/div')
    
    students = []

    for textm in data:

        students.append(textm.text.strip())
        #print(textm)
    #print(students[0])
    #with open("shuju.txt",'a',encoding='utf-8') as f:

    l1=[]
    l2=[]
    l3=[]
    l4=[]
    for i in range(len(students)):
        print(students[i])
        if(students[i]):
            sl=students[i].split("\n")
            for tag in range(len(sl)):
                if tag==0:
                    #tag+=1
                    continue
                if (tag+2)%2==1:
                    l1.append(sl[tag])
                if (tag+2)%2==0:
                    s2=sl[tag].split(" ")
                    l2.append(s2[0])
                    l3.append(s2[1])
                    l4.append(sl[0])               
    flag=0
    while(flag<10):
        max=int(l3[0])
        m=0
        for d in range(len(l3)):
            if(l3[d]=='--'):
                l3[d]=='0'
            if int(l3[d])>max:
                max=int(l3[d])
                m=d
        print(l4[m],l1[m],l2[m],l3[m])
        l3[m]=0
        flag+=1
#n=0
#while n<10:

#t = Timer(10, func)
func()
#t.start()

#    n=n+1
driver.quit()
