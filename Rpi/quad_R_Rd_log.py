#import socket
import serial
import time
from time import sleep
import csv

IMU_DATA = 19
count    = 0
imu      = [0]*IMU_DATA
DATA     = [0]*(IMU_DATA+2)
DATA[0]  = 1
arm_check = 1

def init_file():
    with open(FileName,'w') as csv_file:
        headers = ['Sr. No','time','arm_disarm', 'R11', 'R12','R13','R21','R22','R21','R31','R32','R33', 'Rd11', 'Rd12','Rd13','Rd21','Rd22','Rd21','Rd31','Rd32','Rd33']
        thewriter = csv.writer(csv_file)
        thewriter.writerow(headers)

def write_file():
    global DATA
    thewriter = csv.writer(csv_file)
    thewriter.writerow(DATA)
    DATA[0] += 1
    
def RightUnit():
    global count, arm_check
    startTimeExp = time.time()
    Sport = serial.Serial("/dev/ttyS0", baudrate = 115200, timeout = 1)
    while True:
        try:
            rcv = Sport.readline()
            afterSplit = rcv.split(',')
            #print(rcv)
            for i in range(IMU_DATA):
                imu[i] = float(afterSplit[i])

            DATA[2],DATA[3],DATA[4],DATA[5],DATA[6],DATA[7],DATA[8],DATA[9],DATA[10],DATA[11],DATA[12],DATA[13],DATA[14],DATA[15],DATA[16],DATA[17],DATA[18],DATA[19],DATA[20] = imu[0],imu[1],imu[2],imu[3],imu[4],imu[5],imu[6],imu[7],imu[8],imu[9],imu[10],imu[11],imu[12],imu[13],imu[14],imu[15],imu[16],imu[17],imu[18]

            DATA[1] = time.time() - startTimeExp
            count = count + 1
            #print(imu)
            #arm_check = float(imu[0])
            
            if arm_check == 1:           
                write_file()
                  
        except IndexError:
            print("You Encountered Index Error!!")
            pass
            
        except ValueError:
            print("You Encountered Value Error!!")
            pass
        
        except KeyboardInterrupt:
            print("Keyboard Interrupt")
            print("\n{} Samples acquired in {} seconds".format(count,round(time.time()-startTimeExp,3)))
            break
        
           
if __name__ == "__main__":

    File  = raw_input("Enter the File Name: ")
    FileName = File + '.csv'
    
    init_file()
    csv_file = open(FileName,'a')
    RightUnit()    


    
