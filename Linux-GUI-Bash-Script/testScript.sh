#!/bin/bash

#function to get the host IP
getHostName(){
	hostName=$(hostname -I)
	echo "Host name of system"
	echo "==================="
	host $hostName | awk 'NF>1{print $NF}'
	echo
	echo "User active on this system"
	echo "=========================="
	loginctl 

	
}

checkPassword(){

	read -p "What user would you like to check for passwords? " user
	clear
	passwd -S $user


}

changePassword(){

	passwd
}

#OS detection scan
nmapFunct1(){
	hostName=$(hostname -I)
	sudo nmap -O $hostName
}

#machine details
nmapFunct2(){
	hostName=$(hostname -I)
	nmap -v $hostName

}

#all local ports scan
nmapFunct3(){
	hostName=$(hostname -I) 
	nmap -p 1-65535 $hostName
}

#top 20 most common ports scan
nmapFunct4(){ 
	hostName=$(hostname -I)
	nmap --top-ports 20 $hostName

}

#simple malware scan
nmapFunct5(){
	hostName=$(hostname -I)
	nmap -sV --script=http-malware-host $hostName 

}

systemChecks(){
	echo 0 - EXIT
	echo 1 - Running Services
	echo 2 - Enabled Services
	echo 3 - Disabled Service
	echo 4 - Specific Service Status 
	read -p "Select a service scan to be completed: " choice
	clear
	while [ $choice != "0" ]
	do
		case $choice in
			1)
				echo "Scan for Running Services..."
				sleep 3
				systemctl list-units --type=service --state=running
				read -p "Press enter to continue..."
				clear
				;;
			2)
				echo "Scan for Enabled Service..."
				sleep 3
				systemctl list-unit-files --type=service --state=enabled
				read -p "Press enter to continue..."
				clear
				;;
			3)
				echo "Scanning for Disabled services..."
				sleep 3
				systemctl list-unit-files --type=service --state=disabled
				read -p "Press enter to continue..."
				clear
				;;
			4)
				
				read -p "What Process do you want to scan for? " processVar
				systemctl status $processVar
				read -p "Press enter to continue..."
				clear
				;;
			*)
				echo "Invailid option, please try agian."
				sleep 1
				clear
		esac 
		echo 0 - EXIT
		echo 1 - Running Services
		echo 2 - Enabled Services
		echo 3 - Disable Services
		echo 4 - Specific Service Status
		read -p "Select a service scan to be completed: " choice
		clear
	done

}

nmapMenu(){

	echo 0 - EXIT
	echo 1 - OS Detection/Details
	echo 2 - Machine Details
	echo 3 - Local Ports Scans
	echo 4 - Common Ports Scan
	echo 5 - Malware
	read -p "Select an nmap scan to be completed: " choice
	clear
	while [ $choice != "0" ]
	do
	
		case $choice in
			1)
				echo "Scanning for OS..."
				sleep 3
				nmapFunct1
				echo
				read -p  "Press enter to continue..."
				clear
				;;
			2)
				echo "Retreving Machine Details..."
				sleep 3 
				nmapFunct2
				echo
				read -p "Press enter to continue..."
				clear
				;;
			3)
				echo "Conducting Local Ports Scans..."
				sleep 3
				nmapFunct3
				echo
				read -p "Press enter to continue..."
				clear
				;;
			4)
				echo "Scanning for most 20 common ports..."
				sleep 3
				nmapFunct4
				echo
				read -p "Press enter to continue..."
				clear
				;;
			5)
				echo "Conducting Basic Malware Scan..."
				sleep 3 
				nmapFunct5
				echo
				read -p "Press enter to continue..."
				clear
				;;
			*)
				echo "Invalid option please try again"
				sleep 1
				clear
		esac
		echo 0 - EXIT
		echo 1 - OS Detection/Details
		echo 2 - Machine Details
		echo 3 - Local Ports Scans
		echo 4 - Common Ports Scan
		echo 5 - Malware
		read -p "Scan an nmap scan to be completed: " choice
		clear
	done
}

passwordMenu(){

	echo 0 - Exit
	echo 1 - Password Status
	echo 2 - Password Change
	read -p "Select option to be completed on your password: " choice
	clear
	while [ $choice != "0" ] 
	do
		case $choice in
			1)
				checkPassword
                                echo
                                echo "P - Usable Password"
                                echo "NP - No Password"
                                echo "L - Locked Password"
                                echo "99999 - Never Expires"
                                echo "0 - Can be changed at anytime"
                                read -p "Press enter to continue..."
                                clear
                                ;;
			2)
				read -p "Would to proceed to changing your password? [Y/N]: " answer
                                if [ $asnwer == "Yes" ] || [ $answer == "YES" ] || [ $answer == "Y" ] || [ $answer == "y" ]
                                then
                                        changePassword
                                        read -p "Press enter to continue..."
                                        clear
                                else
                                        echo "Returning to menu..."
                                        clear
                                fi
                                ;;
			*)
				echo "Invalid option please try again."
				sleep 1
				clear
		esac
		echo 0 - EXIT
		echo 1 - Password Status
		echo 2 - Password Change
		read -p "Select an option to be completed on your password: " choice
		clear
	done
}

echo "Installing required packages..."

#packages to be installed in order to run the program
sudo apt install iftop -y
sudo apt install nmap -y
sudo apt install net-tools -y
sudo apt install freeipa-client -y
sudo apt install sysstat -y
sudo apt install chkrootkit -y
clear

	echo 0 - EXIT
	echo 1 - Host Information 
	echo 2 - System Checks
	echo 3 - Nmap Scans
	echo 4 - System Upgrade
	echo 5 - Password Settings
	echo 6 - Rootkit Scan
	read -p "Select action to be completed: " choice
	clear
	while [ $choice != "0" ]
	do
		case $choice in
			1)
				getHostName
				read -p "Press enter to continue..."
				clear
				;;
	
			2)
				systemChecks
				;;
	
			3)
				nmapMenu
				clear
				;;
			4)
				echo "Upgrading your system..."
				sudo apt upgrade
				echo
				read -p "Press enter to continue..."
				clear
				;;
			5)
				passwordMenu
				clear
				;;
			6)
				echo "Running Rootkit scan..."
				sleep 2
				chkrootkit 
				echo
				read -p "Press enter to continue..."
				clear
				;;
	
			*)
				echo "Invalid option, please try again." 
				sleep 1 
				clear
		esac
		echo 0 - EXIT
		echo 1 - Host Information
		echo 2 - System Check
		echo 3 - Nmap Scans 
		echo 4 - Service Checks
		echo 5 - Password Settings
		echo 6 - Rootkit Scan
		read -p "Select an action to be completed: " choice
		clear
	done

