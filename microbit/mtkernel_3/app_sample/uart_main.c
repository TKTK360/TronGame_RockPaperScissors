#include <tk/tkernel.h>  /* T-Kernel ヘッダ */
#include <tm/tmonitor.h> /* tm_printf() など */
#include "config_main.c"


//-----------------------------------------------------------
// UARTE1の初期化とピン設定
//-----------------------------------------------------------
LOCAL void init_uarte1(INT pin_txd, INT pin_rxd)
{
	out_w(UARTE(1, EVENTS_ENDRX), 0);
	out_w(UARTE(1, EVENTS_ENDTX), 0);
	out_w(UARTE(1, EVENTS_ERROR), 0);
	out_w(UARTE(1, ERRORSRC), 0);

	//out_w(UARTE(1, BAUDRATE), 0x00275000);		// 9600 baud(※B)
	out_w(UARTE(1, BAUDRATE), 0x01D7E000);	// 115200 baud(※B)

	// Hardware flow control無し、パリティ無し、One stop bit
	out_w(UARTE(1, CONFIG), 0);

	out_w(UARTE(1, PSEL_TXD), pin_txd);	// TXDのGPIOピン設定(※C)
	out_w(UARTE(1, PSEL_RXD), pin_rxd);	// RXDのGPIOピン設定(※C)
	out_w(UARTE(1, ENABLE), 8);			// Enable UARTE(※D)
}


//-----------------------------------------------------------
// UARTE1の送信
//-----------------------------------------------------------
LOCAL void uarte1_tx(INT txcnt, UB *txadr)
{
	out_w(UARTE(1, TXD_PTR), (UW) txadr);	// 送信バッファ先頭アドレス
	out_w(UARTE(1, TXD_MAXCNT), txcnt);		// 送信するバイト数

	out_w(UARTE(1, EVENTS_ENDTX), 0);		// 送信終了フラグのクリア
	out_w(UARTE(1, TASKS_STARTTX), 1);		// 送信開始(※F)

	// 送信終了までポーリングで待ってから戻る(※G)
	while(! in_w(UARTE(1, EVENTS_ENDTX))) {
		tk_dly_tsk(10);
	}
}


//-----------------------------------------------------------
// UARTE1の受信
//-----------------------------------------------------------
LOCAL void uarte1_rx(INT rxcnt, UB *rxadr)
{
	out_w(UARTE(1, RXD_PTR), (UW) rxadr);	// 受信バッファ先頭アドレス
	out_w(UARTE(1, RXD_MAXCNT), rxcnt);		// 受信するバイト数

	out_w(UARTE(1, EVENTS_ENDRX), 0);		// 受信終了フラグのクリア
	out_w(UARTE(1, TASKS_STARTRX), 1);		// 受信開始(※J)

	// 受信終了までポーリングで待ってから戻る(※K)
	while(! in_w(UARTE(1, EVENTS_ENDRX))) {
		tk_dly_tsk(10);
	}
}
