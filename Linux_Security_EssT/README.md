# Linux Security and Hardening Essential Training

## General Security
* Linux is "secure", but it's not a panacea.
* People play a key role in security.
* Security is an ongoing process.
* Linux security features
	* Open Source.
	* It's not a popular target.
	* Package management.
	* Separation of privileges(multi-user system).
* Security Principles
	* Principles of Least Privilege
	* Use encryption
	* Shared accounts (Yes, root can be a shared account!)
	* Multi-factor authentication
	* Firewall
	* Monitoring logs

## Physical Security

### Protect from Grub `e` edit single user mode
* In systemd go to `/lib/systemd/system/`
* Replace `sushell` with `sulogin` in emergence.service and rescue.service

### Protect Grub by password
* Let's just say username=`grubProtect` and password=`grubzilla123`
* In `/etc/grub.d/40_custom` add

	```
	set superuser="grubProtect"
	password grubProtect grubzilla123
	```

* For encrypted password
		
	```
	grub-mkpasswd-pbkdf2
	```

* Enter the password=`grubzilla123` and get an output like this

	```
	PBKDF2 hash of your password is grub.pbkdf2.sha512.10000.1CAEF371E5B24AF502560031A265F29E0518DF83D930640253B3D938D8DC40271578C2BDF293AE139C5F54D56DAA5800184603ECC5BDAA75A68EBF7304B1C409.DDA7417797A13B4B52197034066C5A1964C2776864DA507F2A73E62D79692C00D86D63353DD936CDB1517EBB1A2729D4252E237E0A2C89433E2CFE4C8AA4D050
	```
	
* In /etc/grub.d/40_custom
		
	```
	set superuser="grubProtect"
	password_pbkdf2 grubProtect grub.pbkdf2.sha512.10000.1CAEF371E5B24AF502560031A265F29E0518DF83D930640253B3D938D8DC40271578C2BDF293AE139C5F54D56DAA5800184603ECC5BDAA75A68EBF7304B1C409.DDA7417797A13B4B52197034066C5A1964C2776864DA507F2A73E62D79692C00D86D63353DD936CDB1517EBB1A2729D4252E237E0A2C89433E2CFE4C8AA4D050
	```

* Then `update-grub`

### Disk Encryption

* Install `cryptsetup`

* Encrypt New Device/Disk

	**Encrypting Disk Will Remove All Data**

	* Fill device with random data

		```  
		sudo shred -v -n 1 <e.g. /dev/sdb , i.e diskname>
		```

	* Now Run `cryptsetup`	and put in the passphrase

		```
		sudo cryptsetup luksFormat <e.g. /dev/sdb , i.e diskname>
		```

	* Open the device and put in the passphrase

		```
		sudo cryptsetup luksOpen <e.g. /dev/sdb , i.e diskname> <e.g. opt, i.e name for the folder>
		```

	* Since here we named the folder `opt`, you can find that disk on `/dev/mapper/opt` 

	* Formating the device 

		```
		sudo mkfs -t ext4 /dev/mapper/opt
		```

	* Close the device

		```
		sudo cryptsetup luksClose opt
		```

	* Updating `/etc/fstab` for mounting while login

		```
		/dev/mapper/opt /opt ext4 defaults 0 0
		```

	* Updating `/etc/crypttab` for asking passphrase while mounting

		```
		opt /dev/sdb none luks
		```

* Encrypt a File or Folder

	* For example Make folder `/data`

		```
		sudo mkdir /data
		```

	* Locate 100mb to a file `opt` in `/data`
		
		```
		sudo fallocate -l 100M /data/opt
		```
	
	* Adding random data to file `opt`
		```
		sudo dd if=/dev/urandom of=/data/opt bs=1M count=100
		```

	* To check the random data in `/data/opt`

		```
		sudo strings /data/opt
		```

	* Now encrypting `/data/opt`

		```
		sudo cryptsetup luksFormat /data/opt
		```

	* Open

		```
		sudo cryptsetup luksOpen /data/opt opt
		```

	* Format	

		```
		sudo mkfs -t ext4 /dev/mapper/opt
		```

	* Mount

		```
		sudo mount /dev/mapper/opt /opt
		```

* Encrypting Device with Data
	* Backup this Device
	* Fill the Device with random data using `shred` or `dd`
	* Encrypting the Device
	* Open the Device
	* Format it, mostly `ext4`
	* Mount and use it

### Disable `Control` + `Alt` + `Delete`

* `Control` + `Alt` + `Delete` in systemd, rebooting your system.
* To disable this

	```
	systemctl mask ctrl-alt-del.target
	systemctl daemon-reload
	```

### Summary
* Physical security threats.
* Physical security guidelines.
* Single user mode defenses.
* Kernel Parameter Protection.
* Disk encryption with LUKS.
* Disabling reboots from `Ctrl+Alt+Del`

## Account Security

### PAM (Pluggable Authentication Modules)

* Location:
	```
	/etc/pam.d
	/etc/pam.d/login
	/etc/pam.d/sshd
	```
		
* Format:
	
	```
	module_interface
	control_flag
	module_name
	module_args
	```

* PAM Modules Interfaces
	* `auth` - Authenticates users.
	* `account` - Verifies if access is permitted.
	* `password` - Changes user's password.
	* `session` - Manages user's sessions.

* PAM Control Flags
	* `required` - Module result must be successful to continue.
	* `requisite` - Like required, but no other modules are invoked.
	* `sufficient` - Authenticates user if no required modules have failed, otherwise iggnored.
	* `optional` - Only used when no other modules reference the interface.
	* `include` - Includes configuration from another file.
	* complext control flags - `attribute=value` for more info `man pam.d`

* PAM configuration
	
	* Configuration:
		
		```
		account required pam_nologin.so
		session required pam_unix.so
		```


	* Getting Help:
		
		```
		man pam_nologin
		man pam_unix
		```

### Linux Account Types

* `root`, the superuser
	* Root can do anything.
	* Always has the UID of 0.

* System Accounts
	* UIDs of System Accounts are  &lt; 1000 
	* Configured in `/etc/login.defs`
	
	* `useradd` with `-r` flag specifies to have UID of System Account Range
	
		```
		useradd -r system_account_name
		```

* Normal User Accounts
	* UIDs of Normal User Accounts are &ge; 1000
	* Intended for human (interactive) use

* Password Security
	* Enforcing, not hope for, strong passwords.
	* Protect against weak use pam_pwquality, based on pam_cracklib.
		* Configuration File:

			```
			/etc/security/pwquality.conf
			```

		* PAM Usage:
			
			```
			password requisite pam_pwquality
			```

		* Module attributes:
		
			```
			man pam_pwquality
			``` 

	* Use Shadow Password
		* Usually encrypted passwords is stored in `/etc/passwd`
		* But `/etc/passwd` is accessible with all users
		* When shadow password is enabled, the passwords at `/etc/passwd` is replaced with `x`
		* And the passwords are stored in /etc/shadow which is only accessible by root or the superuser
		* Converting regular passwords to shadow passwords

			```  
			pwconv
			```

		* Converting shadow passwords to regular passwords
		
			```
			pwunconv
			```

		* Each feild of `/etc/shadow` is separated by `:`
			* Username
			* HashedPassword
			* DaysSinceEpochOfLastPasswordChange
			* Days until change allowed
			* Days before change required
			* Days warning for expiration
			* Days before account inactive
			* Days since epoch when account expires
			* Reserved
	
	* User account expire info
			
		```
		chage -l <account>
		```

### Controlling Account Access

* Locking and Unlocking Accounts

	* Locking

		```
		passwd -l <account>
		```	

	* Unlocking

		```
		passwd -u <account>
		```

* Locking using `nologin` as Shell	

	```
	chsh -s <shell> <account>
	chsh -s /sbin/nologin <account>
	```

### Account Security

* Disable `root` Logins
	
	* Update `/etc/pam_securetty`
		
		```
		auth [user_unknown=ignore success=ok \
		ignore=ignore default=bad] pam_securetty.so
		```
		
		* `/etc/securetty` contains list of devies where `root` is allowed to login
			* Example

			```
			console
			tty1
			``` 

	* Disable SSH root Logins
		
		* Update `/etc/ssh/sshd_config`

			```
			PermitRootLogin no
			``` 
		
		* Then reload `systemctl`
		
			```
			systemctl reload sshd
			```

* System/Application Accounts
	
	* Use one Account per service 
	
		webs service (httpd), web service account (apache) 
	
	* Don't allow direct logins from the account
	
		* Update `/etc/ssh/sshd_config`
		
			```
			DenyUsers <account1> <account2> ... <accountN>
			```
	* Use `sudo` for all access
			
		```
		sudo -u apache apachectl configtest
		```

* Delete Accounts
	* Determine the UID
		
		```
		id <account>
		```
	
	* Delete Account
	
		```
		userdel -r <account>
		```
		
		* The `-r` flag removes the home directory with removing user  

	* Find	files belonging to that account
		
		```
		find / -user <UID>
		findn / -nouser
		```

## Network Security 

### Securing Network Services
* Use a dedicated user for each service.

	Take advantage of privilege separation.

* Ports below 1024 are privileged.

	Use root to open them, then drop privileges.

	Configuration Controlled by each service.

* Stop and unistall unused services
* Avoid unsecure services

	Use SSH instead of telnet, rlogin, rsh, and FTP

* Avoid information leakage or revealing information where possible 
	* Web server banners.
	* Files `/etc/motd`,`/etc/issue`, and `/etc/issue.net`

* Disable and Uninstall services that are not required.

* List Listening Programs with netstat

	```
	sudo netstat -nutlp
	```

* Port Scanning using `nmap`
	
	```
	nmap <hostname/IP>
	nmap localhost
	nmap 10.11.123.23
	``` 

* List open file using `lsof`
	
	`-i` flag shows all listening network files
	
	```
	lsof -i
	```
* Testing a Specific Port 

	* Using `telnet`

		```
		telnet <hostname/IP> <port>
		```

	* Using `nc`

		`-v` flag means its running in verbose mode 

		```
		nc -v <hostname/IP> <port>
		```

* Xinetd Controlled Services
	* Services Controlled by Xinetd could be found in `/etc/xinetd.d/` folder
	* To disable a service update service config file

		```
		disable=yes
		```

### SSH (Secure SHell)

* Allows for key based authentication.
* To allow key-authentication update `/etc/ssh/sshd_config`

	```
	PubkeyAuthentication yes
	```

* Create SSH Keys
	* Use the `ssh-keygen` command to create a key.
	* You can create a passphrase for the key.
	* The `ssh-keygen` command creates `~/.ssh/id_rsa` and `~/.ssh/id_rsa.pub`.
* Adding the Public Key to Remote Host
	* To copy the key, use `ssh-copy-id`:
			
		```
		ssh-copy-id <user>@<host>
		```

	* This adds public key to `~/.ssh/authorized_keys`

* Force only Key Authentication
	* Updating `/etc/ssh/sshd_config`

		```
		PasswordAuthentication no
		```

	* This allowing only authentication with keys 
	* Hence, protecting `ssh` from brute force attacks

* Controlling Root Logins
	* Updating `/etc/ssh/sshd_config`
	* To disable root logins

	 	```
		PermitRootLogin no 
	  ```

	* To only allow root to login with key

		```
		PermitRootLogin without-password
		```

* Allow Only Certain Users and Groups SSH Access
	* Updating `/etc/ssh/sshd_config`

		```
		AllowUsers <user1> <user2> ... <userN>
		AllowGroups <group1> <group2> ... <groupN>
		```
	
* Deny Certain Users and Groups SSH Access
	* Updating `/etc/ssh/sshd_config`

		```
		DenyUsers <user1> <user2> ... <userN>
		DenyGroups  <group1> <group2> ... <groupN>
		```

* Comman `ssh` flag
	* `-L` for SSH Port Forwording 
		
		Basically using host machines port as the host machines by the client
	
		```
		ssh -L <client port>:<host IP>:<host port> <user@host/dns> 
		ssh -L 3306:127.0.0.1:3306 server1
		ssh -L 8080:www.google.com:80 server1
		```
	
	* `-D` for Dynamic Port Forwarding / SOCKS

		Basically forwarding all request to the client port to host port
		
		```
		ssh -D <client port> <user@host/dns>
		ssh -D 8080 server1
		```
	
	* `-R` for Reverse Port Forwarding
	
	 	Basically forward all request from host machine back to client machine

		```
		ssh -R <host port>:<host IP>:<client port> <user@host/dns>
		ssh -R 2222:127.0.0.1:22 server1
		```

* Disable TCP Port Forwording
	* Updating `/etc/ssh/sshd_config`

		```
		AllowTcpForwarding no
		GatewayPorts no
		```	
* Use SSHv2 instead of SSHv1
	* Updating `/etc/ssh/sshd_config`

		```
		Protocol 2
		```
		
* Bind SSH to a Specific Address
	* Updating `/etc/ssh/sshd_config`

		```
		ListenAddress <host/address1>
		ListenAddress <host/address2>
		.
		.
		.
		ListenAddress <host/addressN>
		```
* Change the Default SSH Port
	* Update `/etc/ssh/sshd_config`
		
		```
		Port 2222
		```

* Adding New Port to SELinux
	
	```
	semanage port -a <SSH port> -p tcp <new port>
	semanage port -l | grep ssh
	```
* Avoid Information Leakage
	* Disable the Banner

		Banner data that is usually stored at `/etc/issue.net` which is sent a remote user before authentication is allowed		

		* Update `/etc/ssh/sshd_config`
		
			```
			Banner none
			```

* To Reload the Configuration

	```
	systemctl reload sshd
	```

* More Info

	```
	man ssh
	man sshd
	man sshd_config
	```

## Linux Firewall 

* Firewalls control network access.
* Linux firewall = Netfilter + IPTables
* Netfilter is kernal framework.
* IPTables is packet selection system.
* Use the `iptables` command to control the firewall.

* Default Tables
	* Filter 
		* Most commanly used table.
		* It is used to block incoming or deny outgoing connections
	* NAT
		* Network Address Translation.
		* It allows a single IP address to be shared
	* Managle
		* Alter packets.
	* Raw
		* Rarely used 
		* Used to disable connection tracking
	* Security
		* Used for manadatory access control networking rules 
		* Used by SELinux 

* Default Chains
	* INPUT
	* OUTPUT
	* FORWARD
	* PREROUTING
	* POSTROUTING

* Tables vs Chains 

	||**INPUT**|**OUTPUT**|**FORWARD**|**PREROUTING**|**POSTROUTING**|
	|---|:---:|:---:|:---:|:---:|:---:|
	|**Filter**|x|x|x|||
	|**NAT**|x|x||x|x|
	|**Mangle**|x|x|x|x|x|
	|**Raw**||x||x||
	|**Security**|x|x|x|||

* Rules
	* Rules = Match + Target

	* Match on
		* Protocol
		* Source/Destination IP or Network
		* Source/Destination Port
		* Network Interface
		* Eg: `protocol:TCP, source IP:1.2.3.4, destination port:80` 

	* Targets
		* Chain
		* Built-in targets
			* ACCEPT
			* DROP
			* REJECT
			* LOG
			* RETURN

### Command-Line interface

* Command `iptables`/`ip6tables`
	* `iptables` for IPv4.
	* `ip6tables` for IPv6.
	* List/View
		* Display the filter table.
		
		```
		iptables -L
		```

		* Display the NAT table.
		
		```
		iptables -t nat -L
		```

		* Display using numeric output.

		```
		iptables -nL
		```

		* Display using verbose output.

		```
		iptables -vL
		```

		* Use line numbers.

		```
		iptables --line-numbers -L
		```

	* Creating and Deleting a Chain
		
		* Create Chain

			```
			iptables -t <table> -N <chain>
			```
	
		* Delete Chain

			```
			iptables -t <table> -X <chain>
			```
			
	* Appending, Inserting, and Deleting Rules
		* Appending Rule
			* For appending a rule in the end of chain.
			
			```
 			iptables -A <chain> <rule-specification>
			```
			
			* To specify table use `-t` flag, if not default is filter table.

			```
			iptables -t <table> -A <chain> <rule-specification>
			```
		
		* Inserting Rule
			* For inserting rule at the beginning of the chain.
			
			```
			iptables -I <chain> <rule-specification>
			```	
			
			* After specifing the chain add a rule number to specify where the rule need to be inserted, if not it will default at 0<sup>th</sup> position

			```
			iptables -I <chain> <rule-number> <rule-specification>
			```

		* Deleting Rule
			* For deleting a rule from the chain
			* A rule can be deleted by specify the rule itself or rule number
			
			```
			iptables -D <chain> <rule-specification>	
			iptables -D <chain> <rule-number>	
			```

		* Flushing Rules
			* To delete all rules for a chain
			
			```
			iptables -t <table> -F <chain>
			```

	* Rule Specification Options
	
		|**Option**|**Description**|
		|---|---|
		| -s Source IP</br> -s 10.11.12.13</br> -s 10.11.12.0/24 |Source IP, Network or Name.</br>_Name is resolved when the rule is added._|
		| -d Destination IP</br> -d 10.11.12.13</br> -d 10.11.12.0/24|Destination IP, Network, or Name|
		| -p Protocol</br> -p tcp</br> -p udp</br> -p icmp|Protocol|
		| -m Module module_options|Enable extended packet matching module.</br>(man iptables-extensions)|
		| -p Protocol -m Protocol --sport Port</br> -p tcp -m tcp --sport 8080</br> -p tcp --sport 8080|Source Port|
		| -p Protocol -m Protocol --dport Port</br> -p tcp -m tcp --dport 80</br> -p tcp --dport 80</br> -p udp --dport 53|Destination Port|
		| -p icmp -m icmp --icmp-type Type</br> -p icmp -m icmp --icmp-type echo-reply</br> -p icmp --icmp-type echo-reply </br> -p icmp --icmp-type echo-request |ICMP packet type</br>(iptables -p icmp -h)|
		| -m limit --limit rate [/second/minute/hour/day]</br>-m limit --limit-burst<br> -m limit --limit 5/m --limit-burst 10<br>-m limit ! --limit 5/s|Match until a limit is reached.<br>--limit default is 3/hours<br>--limit-burst default is 5<br>/s = second</br>/m = minute</br>/h = hour</br>/d = day</br>! invert the match |
	
	* Target/Jump
		* To specify a jump point or target
		
		```
		-j <target/chain>
		-j ACCEPT   #Built-in target
		-j DROP     #Built-in target
		-j LOGNDROP #Custom chain
		``` 

* To Save the Rules
	* In Debian and Ubuntu install `iptables-persistent`

		```
		apt install iptables-persistent
		```

	* To save the rules

		```
		netfilter-persistent save
		```

	* Rules and Configuration will be saved in `/etc/iptables`


* `iptables` Examples

	* Drop all connection from source IP of 10.0.0.124
		
		```
		iptables -A INPUT -s 10.0.0.124 -j DROP
		```

	* `-A INPUT` means the rule is being added to the INPUT chain

	* Accepts all TCP connection from source IP of 10.0.0.0/24 and destination port 22

		```
		iptables -A INPUT -s 10.0.0.0/24 -p tcp -dport 22 -j ACCEPT
		```

	* Drops all TCP connection for destination port 22
		
		```
		iptables -A INPUT -p tcp --dport 22 -j DROP
		```

	* To Protect against DOS attacks

		```
		iptables -I INPUT -p tcp --dport 80 \
		-m limit --limit 50/min --limit-burst 200 \
		-j REJECT

		iptables -I INPUT -p tcp --dport 80 \
		-m limit --limit 50/min --limit-burst 200 \
		-m state --state NEW -j REJECT
		```
### TCP Wrappers
* Host-based network ACL system.
* Controls access to "wrapped" services.
* A wrapped service is compiled with libwrap support.
* To Print required shared libraries run
	```
	ldd <path to the binary of the file>
	```
* Can control access by IP address/Networks.
* Can control access by hostname.
* Transparent to the client and service.
* Used with xinetd.
* Centralized management for multiple network services.
* Runtime configuration.
* TCP Wrapper Configuration
	* Configuration files to TCP Wrappers are `/etc/hosts.allow` and `/etc/hosts.deny`
	* When TCP connection request received first `/etc/hosts.allow` is checked.
	* If match is found, access is granted.
	* If not then next `/etc/hosts,deny` is checked.
	* If match is found access is denied and log message will be written.
	* If there are no matches, access is granted.
* TCP Wrappers Examples	
	* The format given below is valid for both `/etc/hosts.allow` and `/etc/hosts.deny`


	```
	# SERVICE(S) : CLIENT(S) [ : ACTION(S) ]
	sshd : 10.11.12.13
	imapd : www.example.com
	sshd, imapd : 10.12.11.13
	ALL : 10.9.8.12, .example.com, .admin.example.com 
	sshd : jumbox*.example.com, jumbox0?.example.com # Regex Matching
	sshd : 10.11.12.
	sshd : 10.
	sshd : 10.11.0.0/255.255.0.0
	sshd : /etc/hosts.sshd # Path to a file with list of host
	imapd : ALL
	```

	* In `/etc/hosts.allow`

		```
		# SERVICE(S) : CLIENT(S) [ : ACTION(S) ]
		sshd : ALL EXPECT .hacker.net  	
		```
	
* TCP Wrappers Logging
		
	```
	# SERVICE(S) : CLIENT(S) [ : ACTION(S) ]
	sshd : 10.11.12.13 : severity emerg
	sshd : 10.11.12.13 : severity local0.alert
	```
		
	* In `/etc/hosts.deny`
		
		```		
		# SERVICE(S) : CLIENT(S) [ : ACTION(S) ]
		sshd : .hacker.net : spawn /usr/bin/wall "Attack in progress."
		sshd : .hacker.net : spawn /usr/bin/wall "Attack from %a."
		```
* Expansions	
	
	|**Format Specifiers**|**Description**|
	|---|---|
	|%a (%A)|The client (server) host address|
	|%c|Client information.|
	|%d|The daemon process name.|
	|%h (%H)|The client (server) host name or address.|
	|%n (%N)|The client (server) host name.|
	|%p|The daemon process id.|
	|%s|Server information.|
	|%u|The client user name (or "unknown").|
	|%%|Expands to single `%` charater.|



## File System Security 

### File and Directory Permissions

```
ls -l 	
```
>-rwxrw-r-- user:group bytes data time filename

* Permission - Files vs Directories
	
	|Symbol|Permission|File|Directory|
	|:---:|---|---|---|
	|r|Read|Allows a file to be read.|Allows file names in the directory to be read.|
	|w|Write|Allows a file to be modified.|Allows entries to be modified within the directory.|
	|x|Execute|Allows the execution of a file.|Allows access to contents and metadata for entries.|

* Permission Categories

	|Symbol|Categories|
	|:---:|---|
	|u|User|
	|g|Group|
	|o|Other|
	|a|All|

* Groups
	* Every user is at one group.
	* Users can belong to  many groups.
	* Groups are used to organize users.
	* The `group` command displays a user's groups.
	* You can also use `id -Gn`


* Secret Decoder Ring
  
	|Type|User|Group|Other|
  |:---:|:---:|:---:|:---:|
	|1|3|3|3|
	|d\\-|rwx|rwx|rwx|
	
	* In Type 
		* Directory is denoted by `d`.
		* File is denoted by `-` or `.`

	* In Users, Groups, and Others the order of permission is (Read , Write m Execute) rwx

	* If any permission is not granted to a file or folder the charater of the permission gets replaced with `-` 
	
* Changing Permission

	|Item|Meaning|
	|---|---|
	|chmod|Change mode command|
	|ugoa|User category</br>user ,group, other, all|
	|+-=| Add, subtract, or set permissions|
	|rwx| Read, Write, Execute|

	```
	chmod u+w filename # User is granted with Write permission
	chmod u-rw filname # User is denied of Read and Write permission
	chmod u+rwx,g-x,o-rwx filname # User is granter with rwx, Groups are denied of x, and Others are denied of rwx
	chmod a=r filname # All (User, Groups, and Others) are Granted with only r permission
	chmod u=rwx,g=rx,o= filename # User has rwx, Groups have rx and Others has None
	```

* Numeric Based Permissions

	||r|w|x|
	|---|:---:|:---:|:---:|
	|**Value for off**|0|0|0|
	|**Binary value for on**|1|1|1|
	|**Base 10 value for on**|4|2|1|

	* Possible Numeric Permissions
		
		|Octal|Binary|String|Description|
		|:---:|:---:|:---:|:---|
		|0|000|\-\-\-|No permissions|
		|1|001|\-\-x|Execute only|
		|2|010|\-w\-|Write only|
		|3|011|\-wx|Write and Execute (2+1)|
		|4|100|r\-\-|Read only|
		|5|101|r\-x|Read and Execute (4+1)|
		|6|110|rw\-|Read and Write (4+2)|
		|7|111|rwx|Read, Write, and Execute (4+2+1)|

* Working with Group
	* New files belong to your primary group.
	* The `chgrp` command changes the group.
	

	```
	chgrp group filename/directoryname
	```

* Directory Permissions Revisited
	* Permissions on a directory can affect the files in the directory.
	* If the file permissions look correct, start checking directory permissions.
	* Work your way up to the root

* File Creation Mask
	* File creation mask determines default permissions.
	* If no mask were used permissions would be:
		* 777 for directory
		* 666 for files

* The `umask` Command
	
	```
	umask [-S] <mode>
	``` 

	*  Sets the file creation mask to mode, if given.
	*  Use -S t symbolic notation.
	
		|	|Directory|File|
		|---|:---:|:---:|
		|Base Permission|777|666|
		|Subtract Umask|\-022|\-022|
		|Creations Permission|755|644|

* Special Modes
	* umask 0022 is the same as umask 022
	* chmod 0644 is the same as chmod 644
	* The special modes are:
		* setuid
		* setgid
		* sticky

* Summary
	* Symbolic permissions.
	* Numeric/octal permissions.
	* File vs Directory permissions
	* Changing permissions.
	* Working with groups.
	* File creation mask.

### Special Modes


* Octal Permissions
	
	||setuid|setgid|sticky|
	|---|:---:|:---:|:---:|
	|Value for of|0|0|0|
	|Binary value for on|1|1|1|
	|Base 10 value for on|4|2|1|

* Setuid
	* When a process is started, it runs using the starting user's UID and GID.
	* setuid = Set User ID upon execution.
	* `-rwsr-xr-x 1 root root /usr/bin/passwd`
	* `ping`
	* `chsh`
	* setuid files are a attack surface.
	* Not honored on shell script
	* Adding the Setuid Attribute

		```
		chmod u+s <filename>
		chmod 4755 <filename>
		``` 

	* Removing the Setuid Attribute
		
		```
		chmod u-s <filename>
		chmod 0755 <filename>
		```

	* Finding Setuid Files

		```
		find / -perm /4000

		# Older style
		find / -perm +4000
		```

	* Only the Owner Should Edit Setuid Files

		||Symbolic|Octal|
		|---|:---:|:---:|
		|**Good**|\-rwsr-xr-x|4755|
		|**Bad**|\-rwsrwxr-x|4775|
		|**Really bad**|\-rwsrwxrwx|4777|

* Setgid
	* setgid = Set Group ID upon execution.
	* `-rwxr-sr-x 1 root tty /usr/bin/wall`
	* `crw--w---- 1 bob  tty /dev/pts/0`
	* Finding Setgid Files

		```
		find / -perm /2000 -ls
		# Older style
		find / -perm +2000 -ls
		``` 
	
	* Adding the Setguid Attribute

		```
		chmod g+s <filename>
		chmod 2755 <filename>
		```
	
	* Adding the Seduid and Setgid Attributes

		```
		chmod ug+s <filename>
		chmod 6755 <filename>
		```
	
	* Removing the Setguid Attribute
	
		```
		chmod g-s <filename>
		chmod 0755 <filename>
		```
	
	* Setgid on Directories
		* setgid on directory causes new files to inherit the group of the directory.
		* setguid causes directories to inherit the setguid bit.
		* Is not retroactive.
		* Great for work with groups.
	
	* Use an Integrity Checker
		* Other to `find`
		* Tripwire
		* AIDE (Advanced Intrusion Detection Environment)
		* OSSEC
		* Samhain
		* Package managers
	
* The Sticky Bit
	* Use on a directory to only allow the owner of the file/directory to delete it.
	* Used on /tmp
	* `drwxrwxrwxt 10 root root 4096 Feb 1 09:47 /tmp`
	* Adding the Sticky Bit
	
		```
		chmod o+t <filename>
		chmod 1777 <filename>
		```

	* Removing the Sticky bit
	
		```
		chmod o-t <filename>
		chmod 0777 <filename>
		``` 
	
	* Reading `ls` Output
		* A capitalized special permission means the underlying normal permission is not set.
		* A lowercase special permission means the underlying normal permission set.
		
		```
		touch file
		chmod 644 file
		ls -l file
		chmod u+s file
		ls -l file
		chmod u+x file
		ls -l file
		```


### File Attributes (xattr)

* Supported by many file systems.
	* `ext2`, `ext3`, `ext4`
	* `XFS`
	* `Btrfs`, `ReiserFS`, `JFS`
	* `OCFS2`, `OrangeFS`, `Lustre`
	* `SquashFS`, `F2FS`

* Attribute: `i` immutable
	* The file cannot be:
		* modified 
		* deleted
		* renamed
		* hard linked to
	* Unset the attribute in order to delete it.

* Attribute: `a` append
	* Append only.
	* Existing contents cannot be modified.
	* Cannot be deleted while attribute is set.
	* Use this attribute on log files.
	* Safeguard the audit trail.
* Other Attributes
	* Not every attribute is supported.
	* `man ext4`,`man xfs`, `man brtfs`, etc.
	* Example: `s` secure deletion

* Viewing Attributes 
	* Use the `lsattr` command.
	

	```
	# lsattr /etc/motd
	--------------- /etc/motd
	# lsattr /var/log/messages.log
	-----a--------- /var/log/messages.log
	```

* Modifying Attributes
	* Use the `chattr` command.
	* `+` adds attributes.
	* `-` removes attributes.
	* `=` sets the exact attributes.

	```
	# lsattr /var/log/messages.log
	--------------- /var/log/messages.log
	# chattr +a /var/log/messages.log
	# lsattr /var/log/messages.log
	-----a--------- /var/log/messages.log
	# chattr -a /var/log/messages.log
	# lsattr /var/log/messages.log
	--------------- /var/log/messages.log
	# chattr =is /var/log/messages.log
	# lsattr /var/log/messages.log
	s---i---------- /var/log/messages.log
	# chattr = /var/log/messages.log
	# lsattr /var/log/messages.log
	--------------- /var/log/messages.log
	```

### ACLs

* ACL = Access Control List
* Provides additional control
* Example: Give one user access to a file.
* Traditional solution is to create another group.
	* Increases management overhead of groups.
	
	```
	groupa: tom, jane
	groupb: tom, jane, bob
	```

* Ensure file system mounted with ACL support

	```
	mount -o acl /path/to/dev /path/to/mount
	tune2fs -o acl /path/to/dev
	```

* Check:

	```
	tune2fs -l /path/to/dev | grep options
	```

* Types of ACLs
	* Access
		* Control access to specific file or directory.
	* Default
		* Used on directories only.
		* Files without access rules use the default ACL rules.
		* Not retroactive.
		* Optional.

* ACLs Can Be Configured:
	* Per user
	* Per group
	* For users not in file's group
	* Via the effective rights mask

* Create ACLs
	* Use the `setfacl` command.
	* May need to install the ACL tools.
	* Modify or add ACLs:


	```
	setfacl -m ACL <filename>
	```

* User ACLs/Rules
	* `u:uid:perms` Set the access ACL for a user.
	

	```
	setfacl -m u:jason:rwx start.sh
	setfacl -m u:sam:xr start.sh
	```

* Group ACLs/Rules
	* `g:uid:perms` Set the access ACL for a group.
	

	```
	setfacl -m g:audio:rwx start.sh
	setfacl -m g:potato:xr start.sh
	```

* Mask ACLs/Rules

	* `m:perms` Set the effective rights mask.
	

	```
	setfacl -m m:rwx start.sh
	setfacl -m m:xr start.sh
	```

* Other ACLs/Rules
	* `o:perms` Sets the access ACL for others.
	

	```
	setfacl -m o:r start.sh
	```


* Create Multiple ACLs at Once
	
	```
	setfacl -m u:bob:r,g:potato:rw start.sh
	```

* Default ACLs
	* `d:[ugo]:perms` Sets the default ACL.


	```
	setfacl -m d:g:potato:rw start.sh
	```

* Setting ACLs Recursively (`-R`)

	```
	setfacl -R -m g:potato:rw start.sh	
	```

* Removing ACLs
	* removing Specific ACL permissions

		* Format

			```
			setfacl -x ACL <filename>	
			```

		* Example
		
			```
			setfacl -x u:jason start.sh
			setfacl -x g:potato start.sh
			```

	* removing ALL ACL permissions

		* Format

			```
			setfacl -b ACL <filename>	
			```

		* Example
		
			```
			setfacl -b start.sh
			```

* Viewing ACLs
	* Format

		```
		getfacl <filename>
		```
	
	* Example
		
		```
		getfacl start.sh
		```

### Rootkit Hunter

* Rootkits
	* Software used to gain root access and remain undetected.
	* They attempt to hide from system administrators and antivirus software.
	* User space rootkits replace common commands such as `ls`, `ps`, `find`, `netstat`, etc.
	* Kernal space rootkits add or replace parts of the core operating systems.
		* Loadable Kernal Modules (LKMs)
		* /dev/kmem
		* /dev/mem

* Rootkit Detection
	* Use a file intergrity checker for user space rootkits. (`AIDE`,`tripwire`,`OSSEC`,etc.)
	* Identify inconsistent behavior of a system.
		* High CPU utilization without corresponding processes.
		* High network load or unusual connections.
	* Kernal mode Rootkits have to be running in order to hide themselves.
	* Halt the system and examine the storage.
		* Use a known good operating system to do the investigation.
		* Use bootable media, for example.

	* `chkrootkit`
		* Shell script that searches for rootkits.
			* Detects modification of system binaries.
			* Checks for promiscuous mode.
			* Checks for missing lastlog and utmp entries.
			* Looks for LKM trojans.
		* Run interactively or schedule execution.
		* [www.chkrootkit.org](http://www.chkrootkit.org)

	* Rootkit Hunter/RKHunter
		* Shell script that searches for rootkits.
		* [rkhunter.sourceforge.net](http://rkhunter.sourceforge.net)

		```
		rkhunter --update
		rkhunter --propupd
		rkhunter -c
		cat /var/log/rkhunter.log
		rkhunter -c -rwo # Warning only
		rkhunter --cronjob
		```

		* Rootkit Hunter Configuration
			* Config File `/etc/rkhunter.conf`
			* For reporting warnings in mail update config file

				```
				MAIL-ON-WARNING=username@domain.com
				```

			* For allowing warning in a hidden files or directories

				``` 
				ALLOWHIDDENDIR="/dev/.udev"
				ALLOWHIDDENFILE="/dev/.blkid.tab"
				ALLOWHIDDENFILE="/dev/.blkid.tab.old"
				```

	* OSSEC 
		* Host Intrusion Detection System (HIDS)
		* More than just Rootkit detection: log analysis, file intergrity checking, alerting.
		* Syscheck module - user mode rootkit detection. (through hash checking of binary files)
		* Rootcheck Module - both user mode and kernel mode rootkit detection.
		* Searches for file names known to be associated with user mode rootkits.
		* Signature based rootkit detection.
		* Queries the OS for information and looks for inconsistent results.
			* Compares netstat with bind() results.
			* Many other checks.
		* [ossec.github.io](http://ossec.github.io/)

* Rootkit Removal
	* Keep a copy of the data if possible.
	* Learn how to keep it from happening again.
	* Reinstall core OS components and start investigating.
		* Not recommended. Easy to make mistake.
	* Safest is to reinstall the OS from a trusted media.
	
* Rootkit Prevention
	* Use good security practices
		* Physical
		* Account
		* Network
	* Use file integrity monitoring
		* AIDE
		* Tripwire
		* OSSEC
	* Keep your systems patched.

## Additional Resources

### Linux Hardening Guides

* [LinuxTrainingAcademy.com/hardening](http://LinuxTrainingAcademy.com/hardening)
* [benchmarks.cisecurity.org](http://benchmarks.cisecurity.org)
* [docs.fedoraproject.org](http://docs.fedoraproject.org)
* [sans.org](http://sans.org)
* [access.redhat.com](http://access.redhat.com)
* [iase.disa.mil/stigs](http://iase.disa.mil/stigs)
* [nsa.gov](http://nsa.gov)
* [giac.org](http://giac.org)
* [suse.com](http://suse.com)
* [help.ubuntu.com](http://help.ubuntu.com)

### Linux Security Email Lists

* [LinuxTrainingAcademy.com/security-lists](http://LinuxTrainingAcademy.com/security-lists)

### Security Meetups

* [LinuxTrainingAcademy.com/citysec](http://LinuxTrainingAcademy.com/citysec)
* [meetup.com](http://meetup.com)

### Security Conference Videos

* [LinuxTrainingAcademy.com/security-videos](http://LinuxTrainingAcademy.com/security-videos)
