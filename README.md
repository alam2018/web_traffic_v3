# Web Traffic Model
We have developed this traffic generator to create traffics according to different traffic profiles for different network slices
This traffic generator can generate www (web) traffic
To model the traffic generation by the users, from the gNB perspective, we propose a superimposed traffic stream at the gNB from the user application 
Single slice can be deployed using multiple cells. Therefore, the total traffic generation by the traffic generator represents users traffic from multiple cells
There are various number of users in the simulation
Each user is deployed and start transmission using a software thread
The packet sizes are pareto distributed. The distribution cut-off is in between 1500 and 50 (maximum and minimum packet size). 
The waiting time between two consecutive packet arrival is normal distributed. The distribution cut-off is in between 3.9ms and 0.001ms (maximum and minimum waiting time). The average waiting time is 1.95ms
The packet arrive in a packet call. There are on average 5 packet calls per session during the whole simulation runtime. Each session can have maximum 25 to minimum 1 packet call per session
Each packet call can have average 25 packets per packet call at each session. The maximum and minimum number of packets per packet call is 30 to 1
Average waiting time between packet calls are 412s during the whole simulation runtime. Maximum 450s to minimum 60s packet call duration is possible per session
Each user has its own session profile. Therefore, the packet arrival from these users get influenced on different observation period because of the different simulation parameters (e.g. different number of packet calls and packets in a packet call, packet inter arrival time, packet call inter arrival time)
Sessions are continuously arriving without any delay
We assume that the transmission of user data is done without any throttling (e.g. fixed connection speed)
The receiver is running on a independent software thread. At the end of each observation period (1s), the receiver calculates the amount of bytes received from all the users
