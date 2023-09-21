daemon: daemon.c
# Compile the program
	gcc -o daemon daemon.c
# Move the executable file to the /usr/sbin directory
	sudo mv daemon /usr/sbin/
# Copy "daemon_auto.service", in the /etc/systemd/system directory
	sudo cp daemon_auto.service /etc/systemd/system
# Reload the systemd daemon 
	sudo systemctl daemon-reload
# Enable the service to start at boot time 
	sudo systemctl enable daemon_auto.service
# Start the service 
	sudo systemctl start daemon_auto.service