# Manage-Privileges
Managing privileges complexities and problems encountered during computer security work.

This program was written as an assignment for Computer Security course.

A computer science student is helping out on a project that involves monitoring a network. The student’s job is to write a program that will pluck the URLs for all HTTP traffic from the network. This requires root privileges, but for policy reasons the professor who is running the project cannot give the student those privileges.
The professor’s solution is to create a small program, called runpriv, that will make a second program called sniff, that the student will write, setuid to root. This way, the student can write the program to get the URLs from the network, and execute it, without having root permissions and without asking a system administrator to make the program privileged at each iteration.
The program runpriv works as follows:
(a) Check that the student is running the program by comparing the real UID of the process with that of the
student. (Assume you are the student for this testing.) If the test fails, print an error message and exit.
(b) Prompttheuserforhisorherpassword,andvalidateitagainsttheauthenticationcredentialintheUCDavis Central Authentication System (use the program kinit(1) for this). If the password entered is incorrect, print an error message and exit.
(c) If the current working directory does not contain a file called sniff, print an error message and exit.
(d) If sniff is not owned by the student, or is not executable by the owner of the file, or can be read, written, or executed by anyone else (except, of course, root), print an error message and exit. This step checks that the student owns the file; that the student can execute it; and that no-one else has any rights over it.
(e) If sniff was created or modified over 1 minute ago, print an error message and exit.
(f) Change the ownership of sniff to root (UID 0), its group to proj (GID 95), and its protection mode to 4550 (meaning setuid to owner, and only readable and executable by the owner and group members — when you call chmod(2), you must have the leading “0”, or you will get strange results. For this exercise, you must use the chown(1) program to change the owner and group. This means you must execute that program from runpriv, giving the appropriate arguments. (In the CSIF, the command will fail because there is no group “proj” — just let the error message print, and continue.)
