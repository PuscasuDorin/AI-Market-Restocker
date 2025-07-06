import subprocess
import psutil
# Install this first npm install -g live-server
# pip install psutil

serverIsRunning = False
print("1)Start the localhost server for the online store!")
print("2)Run main program!")
print("If you want to exit type 'quit'!")

def kill_host():
    parent = psutil.Process(site.pid)
    children = parent.children(recursive=True)
    for child in children:
        child.kill()
    parent.kill()

while True:
    cmd = input(">>").strip().lower()
    if cmd == "1" and not serverIsRunning:
        site=subprocess.Popen('cd /SiemensTeamPoject/AI-Market-Restocker/Online_Store && live-server --port=5500', shell=True, stdout=subprocess.DEVNULL)
        #site = subprocess.Popen(['live-server', '--port=5500'], cwd='/SiemensTeamPoject/AI-Market-Restocker/Online_Store')
        serverIsRunning = True
        print("SITE IP ADDRESS: " + subprocess.run(['hostname', '-I'], capture_output=True, text=True).stdout.strip() + ":5500")  
    elif cmd == "2" and not serverIsRunning:
        print("You have to start the localhost server!")
    elif cmd == "2" and serverIsRunning:
        try:
            subprocess.run(["python3", "main.py"], cwd='/SiemensTeamPoject/AI-Market-Restocker/AI_Sorter')
        except Exception as e:
            print("Not Working")
    elif cmd == "quit":
        kill_host()
        break
    else:
        print("Invalid Command!")