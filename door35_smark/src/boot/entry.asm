;===============Define Code Segment===========================================
CODE_RESET      SEGMENT PARA PUBLIC 'ENTRY'
CODE_RESET	ENDS

extrn	boot : far

CODE_RESET      SEGMENT PARA PUBLIC 'ENTRY'
                ASSUME  CS:CODE_RESET
		public _Entrypoint
_Entrypoint:
		
		JMP     FAR PTR boot
CODE_RESET      ENDS



;===============Program End===================================================                
		END	_ENTRYPOINT
		END