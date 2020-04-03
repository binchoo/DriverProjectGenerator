#************ USER PARAMETERS ************#
name 		:= generated
count 		:= 1
dest		:= .
license 	:= GPL

#************ CONFIGURATIONS ************#
FILE_NAME	:= cdev_$(name)
PROJECT_DIR	:= $(dest)/$(name)_project
CDEV_C_TEMPLATE	:= templates/cdev/cdev.c
CDEV_H_TEMPLATE	:= templates/cdev/cdev.h
CDEV_MK_TEMPLATE:= templates/cdev/cdev_make
CDEV_C	 	:= $(PROJECT_DIR)/$(FILE_NAME).c
CDEV_H	 	:= $(PROJECT_DIR)/$(FILE_NAME).h
CDEV_MK	 	:= $(PROJECT_DIR)/Makefile
	
cdev: makedir cdevc cdevh cdevmk

makedir:
	mkdir $(PROJECT_DIR)

cdevc:
	cp $(CDEV_C_TEMPLATE) $(CDEV_C)
	sed -i 's/{dev_name}/$(name)/g' $(CDEV_C)
	sed -i 's/{dev_count}/$(count)/g' $(CDEV_C)
	sed -i 's/{license}/\"$(license)\"/g' $(CDEV_C)
	sed -i 's/{header}/$(FILE_NAME).h/g' $(CDEV_C)

cdevh:
	cp $(CDEV_H_TEMPLATE) $(CDEV_H)

cdevmk:
	cp $(CDEV_MK_TEMPLATE) $(CDEV_MK)
	sed -i 's/{object_file}/$(FILE_NAME).o/g' $(CDEV_MK)
	sed -i 's/{dev_name}/$(name)/g' $(CDEV_MK)
