#include <tk/tkernel.h>  /* T-Kernel ヘッダ */
#include <tm/tmonitor.h> /* tm_printf() など */


// 以前のボタンの状態を保持する変数
LOCAL BOOL prev_a = FALSE;
LOCAL BOOL prev_b = FALSE;


//-------- ボタンスイッチに対応するGPIO P0のピンを入力に設定 ----
LOCAL void btn_init(void)
{
	out_w(GPIO(P0, PIN_CNF(14)), 0);	// ボタンスイッチAはGPIO P0.14に接続
	out_w(GPIO(P0, PIN_CNF(23)), 0);	// ボタンスイッチBはGPIO P0.23に接続
}


//---------------------------------------------------------------
// ボタンの状態変化のチェックと送信(※T)
LOCAL BOOL check_btn(BOOL new_btn, BOOL *prev_btn)
{
	if(new_btn ==  *prev_btn) {
		return FALSE;	// 状態変化が無ければ何もせずに戻る
	}

	*prev_btn = new_btn;

	return TRUE;
}
