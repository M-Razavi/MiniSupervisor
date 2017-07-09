# MiniSupervisor

      Created on: 	Nov 9, 2013
      Edit Date : 	Nov 9, 2013
 
 In the limited linux kernel which is build for some embedded board (such as 9G20, 9260 ARM processor), No daemon or service management      provided. So to check the status of daemon and re run them, This application could be useful.
 * It reads all directories in /proc
 * If the directory name is numeric, cats cmdline to get the process name.(the numeric foldername is the PID)
 * If the process name coressponds with the searched process name (strstr or strcmp, case sensitive or not), it returns the pid.
 * If it doesn't find the process, it returns -1, if it cannot access /proc it returns -2.
 * If several processes with the same name run, it returns the one with the lower number
