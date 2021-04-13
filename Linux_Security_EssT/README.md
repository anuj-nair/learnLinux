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
		| -s Source IP</br></t> -s 10.11.12.13</br></t> -s 10.11.12.0/24 |Source IP, Network or Name.</br>_Name is resolved when the rule is added._|
		| -d Destination IP</br></t> -d 10.11.12.13</br></t> -d 10.11.12.0/24|Destination IP, Network, or Name|
		| -p Protocol</br></t> -p tcp</br></t> -p udp</br></t> -p icmp|Protocol|
		| -m Module module_options|Enable extended packet matching module.</br>(man iptables-extensions)|
		| -m limit --limit rate [/second/minute/hour/day]</br>-m limit --limit-burst<br> -m limit --limit 5/m --limit-burst 10<br>-m limit ! --limit 5/s|Match until a limit is reached.<br>--limit default is 3/hours<br>--limit-burst default is 5<br>/s = second</br>/m = minute</br>/h = hour</br>/d = day</br>! invert the match |
	
	* Target/Jump
		* To specify a jump point or target
		
		```
		-j <target/chain>
		-j ACCEPT   #Built-in target
		-j DROP 		#Built-in target
		-j LOGNDROP #Custom chain
		``` 
## File System Security 
