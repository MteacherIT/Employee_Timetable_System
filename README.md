# **Employee-Timetable-System** - COMP 4900
~~--------------------------------------~~

Our application simulates employees at a company who are booking shifts for themselves in the company timetable. These employees must be able to book shifts while adhering to predefined seniority prioritisation, the maximum number of hours they can work, and the company's hours of operation. Furthermore, the system must be able to handle the following concurrently: multiple active employees, new employee enrolment, and employee attrition. 

<br>## **Authors:**<br/>
<br>*Billal Ghadie*, 
*Yousuf Ghanem*, 
<br>*Maryam Khalaf*, 
*Tina Vashistha* <br/>
<br>~~--------------------------------------~~<br/>

To execute the simulation, first copy the "registered_employees.txt" and "weekly_shifts.txt" files into a shared location of the QNX Neutrino VM (i.e., /tmp/). Then, use the make file to build the project. Finally, simply run the server process, followed by all the use case processes to run the simulation. There are 3 use case executables to run, its best to run them in the following order:
1. bgh_yg_UseCases - this file executes the use cases written by Billal and Yousuf
2. mkhUseCases     - this file executes the use cases written by Maryam
3. UseCases67      - this file executes the use cases written by Tina

Upon completion, the "stopServer" process can be executed to terminate the server and save the data to their respective files. Alternatively, the "useCasesWithMenu" executable can be called, in place of the use cases, to simulate a GUI. That is, using this executable individual use cases can be called, one at a time, to test the functionality of the simulation.

The following files are part of the simulation:
- Makefile
- README.md
- bgh_yg_UseCases.c
- Menu_bgh_yg_UseCases.c
- mkhUseCases.c
- server.c
- stopServer.c
- registered_employees.txt
- weekly_shifts.txt
- server.h
- shift.h
- employee.h
- time.h

The following files are depreciated. They remain part of the packaging (1) for traceability; and (2) to give an idea of the development cycle/work done as part of our final project:
- shift.h
- supervisor.c
- supervisor.h
- techSupport.h
- time.h
