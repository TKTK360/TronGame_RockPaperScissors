#include <tk/tkernel.h>  /* T-Kernel ヘッダ */
#include <tm/tmonitor.h> /* tm_printf() など */


//-------- LED制御のためのGPIOの初期設定 ------------------------
LOCAL void led_init(void)
{
	// COL1..COL5とROW1..ROW5に接続されたGPIOピンを出力に設定
	out_w(GPIO(P0, PIN_CNF(28)), 1);	// GPIO P0.28(COL1に接続)
	out_w(GPIO(P0, PIN_CNF(11)), 1);	// GPIO P0.11(COL2に接続)
	out_w(GPIO(P0, PIN_CNF(31)), 1);	// GPIO P0.31(COL3に接続)
	out_w(GPIO(P1, PIN_CNF(05)), 1);	// GPIO P1.05(COL4に接続)
	out_w(GPIO(P0, PIN_CNF(30)), 1);	// GPIO P0.30(COL5に接続)
	out_w(GPIO(P0, PIN_CNF(21)), 1);	// GPIO P0.21(ROW1に接続)
	out_w(GPIO(P0, PIN_CNF(22)), 1);	// GPIO P0.22(ROW2に接続)
	out_w(GPIO(P0, PIN_CNF(15)), 1);	// GPIO P0.15(ROW3に接続)
	out_w(GPIO(P0, PIN_CNF(24)), 1);	// GPIO P0.24(ROW4に接続)
	out_w(GPIO(P0, PIN_CNF(19)), 1);	// GPIO P0.19(ROW5に接続)
}


//-------- GPIOの特定のピンpinの出力をval(0または1)に設定 -------
//	pinで指定された以外のピンの状態は変化しない
LOCAL void out_gpio_pin(UW port, UW pin, UW val)
{
	INT port_addr;

	if(port == 1){							// P1の場合
		if(val)
			port_addr = GPIO(P1, OUTSET);	// P1でvalが1の場合
		else
			port_addr = GPIO(P1, OUTCLR);	// P1でvalが0の場合

	} else	{								// P0の場合
		if(val)
			port_addr = GPIO(P0, OUTSET);	// P0でvalが1の場合
		else
			port_addr = GPIO(P0, OUTCLR);	// P0でvalが0の場合
	}
	out_w(port_addr, (1 << pin));			// 指定のピンに1を出力
}


// ビットパターンbitptnの指定のビットに応じてGPIOの指定のピンを制御
//	bitptnの最下位からnビット目(n=1..5)が1の場合はピンpinの出力をvalに設定
//	そのビットが0の場合はvalの反対の値(1→0,0→1)をピンpinの出力に設定
LOCAL void bitptn_gpio_pin(UW bitptn, UW n, UW port, UW pin, UW val)
{
	if(bitptn & (1 << (n - 1))){			// bitptnの最下位からnビット目をチェック
		out_gpio_pin(port, pin, val);		// 1の場合はGPIOにvalを出力(※H)
	} else {
		out_gpio_pin(port, pin, (! val));	// 0の場合はvalの反対の値を出力(※J)
	}
}


// 5ビットのビットパターンbitptnに応じてLEDのCOL1..COL5を制御(※K)
LOCAL void out_led_col(UW bitptn)
{
	// 最下位から5番目のビットが1の場合にGPIO P0.28(COL1)を0に設定(※L)
	bitptn_gpio_pin(bitptn, 5, 0, 28, 0);

	// 最下位から4番目のビットが1の場合にGPIO P0.11(COL2)を0に設定(※L)
	bitptn_gpio_pin(bitptn, 4, 0, 11, 0);

	// 最下位から3番目のビットが1の場合にGPIO P0.31(COL3)を0に設定(※L)
	bitptn_gpio_pin(bitptn, 3, 0, 31, 0);

	// 最下位から2番目のビットが1の場合にGPIO P1.05(COL4)を0に設定(※L)
	bitptn_gpio_pin(bitptn, 2, 1, 5, 0);

	// 最下位のビットが1の場合にGPIO P0.30(COL5)を0に設定(※L)
	bitptn_gpio_pin(bitptn, 1, 0, 30, 0);
}


// ROW1..ROW5のいずれか(rowで番号指定)に接続されたGPIOピンにvalを設定(※N)
LOCAL void out_row_gpio(UW row, UW val)
{
	switch(row){
	  case 1:
		out_gpio_pin(0, 21, val);		// GPIO P0.21(ROW1に接続)にvalを設定
		return;
	  case 2:
		out_gpio_pin(0, 22, val);		// GPIO P0.22(ROW2に接続)にvalを設定
		return;
	  case 3:
		out_gpio_pin(0, 15, val);		// GPIO P0.15(ROW3に接続)にvalを設定
		return;
	  case 4:
		out_gpio_pin(0, 24, val);		// GPIO P0.24(ROW4に接続)にvalを設定
		return;
	  case 5:
		out_gpio_pin(0, 19, val);		// GPIO P0.19(ROW5に接続)にvalを設定
		return;
	}
}


// rowで指定した行のみを点灯させるためのGPIOピンの設定(※P)
//	ROW1..ROW5に接続されたGPIOピンのうち、
//	rowで指定した1本のみを1に設定し、他は0に設定する
LOCAL void set_row_gpio(UW row)
{
	UW	cnt, val;
	for(cnt = 1; cnt <= 5; cnt++){
		out_row_gpio(cnt, ((cnt == row) ? 1 : 0));	// 指定の1本のみ1を設定(※Q)
	}
}
