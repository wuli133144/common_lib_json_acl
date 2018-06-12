HOMEDIR = ./
SUBDIR = ./src/time/ ./src/sign/ ./src/dict/ ./src/log/ ./src/sync/ ./src/net/ ./src/conf \
	 ./src/thread/ ./src/container/ ./src/utils ./src/http ./src/segment \
	 ./src/encoding ./src/mysql ./src/chcode ./src/exception ./src/zip \

include ./Makefile.am

all :
	@for dir in $(SUBDIR); do \
		$(MAKE) -C $$dir -f Makefile; \
		if [ $$? -eq 0 ] ; then \
			echo -e "\033[34;1mMAKE IN $$dir OK. \n\033[m"; \
		else \
			echo -e "\033[31;1mWHEN MAKE IN $$dir ERROR OCCUR.\033[m"; \
			break; \
		fi \
	done

clean :
	@for dir in $(SUBDIR); do \
		$(MAKE) -C $$dir -f Makefile clean; \
		if [ $$? -eq 0 ] ; then \
			echo -e "\033[34;1mMAKE CLEAN IN $$dir OK. \n\033[m"; \
		else \
			echo -e "\033[31;1mWHEN MAKE CLEAN IN $$dir ERROR OCCUR.\033[m"; \
			break; \
		fi \
	done
	#find include -name '*.h' -exec rm -rf {} \;
	#find lib -name '*.a' -exec rm -rf {} \;
