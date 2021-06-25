#!/bin/sh
#Black        0;30     Dark Gray     1;30
#Red          0;31     Light Red     1;31
#Green        0;32     Light Green   1;32
#Brown/Orange 0;33     Yellow        1;33
#Blue         0;34     Light Blue    1;34
#Purple       0;35     Light Purple  1;35
#Cyan         0;36     Light Cyan    1;36
#Light Gray   0;37     White         1;37
RED='\033[0;31m'
GREEN='\033[0;32m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color
# FORMAT bash/zsh : ARG=$(ruby -e "puts (-3..1).to_a.shuffle.join(' ')"); ./push_swap $(echo -n $ARG)
# FORMAT bash/sh : ARG=`ruby -e "puts (1..100).to_a.shuffle.join(' ')"`; ./push_swap $ARG
########################################################################################################################
echo "##### ${RED}TEST PIPEX${NC} #####"
########################################################################################################################
echo "### ${PURPLE}ERROR MANAGEMENT${NC} ###"

echo "# ${GREEN}TOO FEW ARGUMENTS:${NC}"
echo "${CYAN}./pipex file1 \"ls -l\" file3${NC}" && ./pipex file1 "ls -l" file3
echo "${CYAN}./pipex${NC}" && ./pipex

echo "# ${GREEN}FILE RIGHTS ARE INCORRECT:${NC}"
echo "${CYAN}chmod 000 file1${NC}" && chmod 000 file1
echo "${CYAN}./pipex file1 \"cat -e\" \"grep a\" file3${NC}" && ./pipex file1 "cat -e" "grep a" file3
chmod 777 file1

echo "# ${GREEN}FILE DOES NOT EXIST:${NC}"
echo "${CYAN}./pipex blabla \"cat -e\" \"grep a\" file3${NC}" && ./pipex blabla "cat -e" "grep a" file3

echo "# ${GREEN}BINARY DOES NOT EXIST:${NC}"
echo "${CYAN}./pipex file1 \"rabbit\" \"grep a\" file3${NC}" && ./pipex file1 "rabbit" "grep a" file3
echo "${CYAN}./pipex file1 \"cat -e\" \"hole\" file3${NC}" && ./pipex file1 "cat -e" "hole" file3

echo "# ${GREEN}BINARY RIGHTS ARE INCORRECT:${NC}"
touch myprog
chmod -x myprog
echo "${CYAN}./pipex file1 \"./myprog\" \"grep a\" file3${NC}" && ./pipex file1 "./myprog" "grep a" file3
echo "${CYAN}./pipex file1 \"./myprog\" \"ls -l\" file3${NC}" && ./pipex file1 "./myprog" "ls -l" file3

########################################################################################################################
#echo "### ${PURPLE}IDENTITY TEST${NC} ###"
########################################################################################################################
#echo "${CYAN}./pipex file1 \"cat -e\" \"grep a\" file3${NC}" && ./pipex file1 "cat -e" "grep a" file3

########################################################################################################################
#echo "### ${PURPLE}SIMPLE VERSION${NC} ###"
########################################################################################################################
#echo "${CYAN}./pipex file1 \"cat -e\" \"grep a\" file3${NC}" && ./pipex file1 "cat -e" "grep a" file3


########################################################################################################################
#echo "### ${PURPLE}ANOTHER SIMPLE VERSION${NC} ###"
########################################################################################################################
#echo "${CYAN}./pipex file1 \"cat -e\" \"grep a\" file3${NC}" && ./pipex file1 "cat -e" "grep a" file3

########################################################################################################################
echo "##### ${RED}END TEST PIPEX${NC} #####"
########################################################################################################################