help: 
	@echo "==============================="
	@echo "make INGRESO_MAGIOS"
	@echo "make ASCENCION_MAGIOS"
	@echo "make BATALLA_NAVAL"
	@echo "==============================="
    
INGRESO_MAGIOS:
	@./TP0_Iniciación/magios

ASCENCION_MAGIOS:
	@./TP1_Rito_Ascención/bibliotecas_tp1/juego

BATALLA_NAVAL:
	@./TP2_Venciendo/batalla_naval ./TP2_Venciendo/barcos.csv ./TP2_Venciendo/reportes.csv
