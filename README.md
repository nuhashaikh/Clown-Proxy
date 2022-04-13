# Clown-Proxy
before compiling -> Configure

-   Make sure to change the SERVEPORT number to an unused 5 digit port on your machine

-   Go to a google chrome incognito window

-   Click the menu button  on the top right corner

**Configure & Compile**

-   Click the settings option

-   Type "proxy" in the search bar

-   Click on the last option which says "Oper your computer's proxy settings"

-   The following steps may look different depending on your OS(Windows steps but should be similar):

-   In your computer settings enable the use of a manual proxy server

-   Enter the address of your server as 136.159.5.25 which is the ip address of the Linux machine 

-   Enter the SERVEPORT number you had changed in the proxy_two.c file

Compile (typing following commands in terminal)

1.  gcc proxy_two.c -o proxy

2.  ./proxy

**Use**

1.  Open an incognito window in chrome

2.  Make sure to close all other tabs to help keep your terminal clear and not spam it with all different and irrelevant browser requests

3.  Paste any of the following test case links in Table 1.1 and refer to the proxy changes for the functionality of the proxy

4.  If you encounter error code "304" please hold shift and press the refresh button for your desired web page

5.  If you would like to close the program press Ctrl+C and the program will end

6.  If want to run again choose a different number for SERVEPORT

**all testing was done on the Linux servers through my local machine at home using Chrome**
