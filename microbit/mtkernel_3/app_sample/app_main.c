#include <tk/tkernel.h>
#include <tm/tmonitor.h>
#include "config_main.c"
#include "uart_main.c"
#include "led_main.c"
#include "button_main.c"


// オブジェクトID番号
ID txtskid;				// 送信側タスク
ID rxtskid;				// 受信側タスク

// Microbit - M5 Atom
//  P0 [TX]   [RX] 22
//  P1 [RX]   [TX] 19

// UARTEの送信(TX)と受信(RX)に使用するエッジコネクタ端子の定義(※M)
#ifdef P0TX_P1RX
const INT tx_pin = 2;	// 送信をGPIO P0.02=端子P0(RING0)に設定
const INT rx_pin = 3;	// 受信をGPIO P0.03=端子P1(RING0)に設定
#endif

#ifdef P1TX_P0RX
const INT tx_pin = 3;	// 送信をGPIO P0.03=端子P1(RING0)に設定
const INT rx_pin = 2;	// 受信をGPIO P0.02=端子P0(RING0)に設定
#endif

static W	i_time;				// システム起動時刻

// UARTEの送信データと受信データを入れるバッファメモリ
#define	MAXBUF	5
static _UB	txbuf[MAXBUF];
static _UB	rxbuf[MAXBUF];

// ゲームパターン
#define M_Title  1
#define M_Main   2
#define M_End 	 3

UB Mpattern = M_Title;

// ボタンパターン
#define Btn_U    0
#define Btn_A    1
#define Btn_B    2

UB Bpattern = Btn_U;

// SOUNDパターン
#define S_UNKNOWN  0
#define S_GYANKEN  1
#define S_AIKO     2
#define S_YOU_WIN  3
#define S_YOU_LOST 4

// LEDパターン
#define L_UNKNOWN  0
#define L_RESET    1
#define L_ROCK     2
#define L_PAPER    3
#define L_SCISSORS 4
#define L_WIN 	   5
#define L_LOSE 	   6
#define L_EVEN 	   7


W  led_disp_row = 1;		// 現在表示中のLEDの行の番号(1..5)
UW soundIndex = S_UNKNOWN;
UB GyankenOwner = 0;
UB GyankenEnemy = 0;

INT rx_current = 0;
INT rx_old = 0;

//LED Pattern
// Reset
LOCAL UB ledptn_reset[] = {
	//___■■■■■
	0b00000000,		// ■ ■ ■ ■ ■
	0b00000000,		// ■ ■ ■ ■ ■
	0b00000000,		// ■ ■ ■ ■ ■
	0b00000000,		// ■ ■ ■ ■ ■
	0b00000000		// ■ ■ ■ ■ ■
};

// Win
LOCAL UB ledptn_win[] = {
	//___■■■■■
	0b00010101,		// ■ □ ■ □ ■
	0b00010101,		// ■ □ ■ □ ■
	0b00010101,		// ■ □ ■ □ ■
	0b00001010,		// □ ■ □ ■ □
	0b00001010		// □ ■ □ ■ □
};

// Lose
LOCAL UB ledptn_lose[] = {
	//___■■■■■
	0b00010000,		// □ ■ □ □ □
	0b00010000,		// □ ■ □ □ □
	0b00010000,		// □ ■ □ □ □
	0b00010000,		// □ ■ □ □ □
	0b00011111		// □ ■ ■ ■ ■
};

// Aiko
LOCAL UB ledptn_even[] = {
	//___■■■■■
	0b00000000,		// ■ ■ ■ ■ ■
	0b00000000,		// ■ ■ ■ ■ ■
	0b00011111,		// □ □ □ □ □
	0b00000000,		// ■ ■ ■ ■ ■
	0b00000000		// ■ ■ ■ ■ ■
};

// Guu
LOCAL UB ledptn_rock[] = {
	//___■■■■■
	0b00000000,		// ■ ■ ■ ■ ■
	0b00001110,		// ■ □ □ □ ■
	0b00001111,		// ■ □ □ □ □
	0b00001110,		// ■ □ □ □ ■
	0b00000000		// ■ ■ ■ ■ ■
};

// Paa
LOCAL UB ledptn_paper[] = {
	//___■■■■■
	0b00000100,		// ■ ■ □ ■ ■
	0b00001110,		// ■ □ □ □ ■
	0b00011110,		// □ □ □ □ ■
	0b00001111,		// ■ □ □ □ □
	0b00001110		// ■ □ □ □ ■
};

// Choki
LOCAL UB ledptn_scissors[] = {
	//___■■■■■
	0b00010001,		// □ ■ ■ ■ □
	0b00001010,		// ■ □ ■ □ ■
	0b00001110,		// ■ □ □ □ ■
	0b00001110,		// ■ □ □ □ ■
	0b00000100		// ■ ■ □ ■ ■
};


//-----------------------------------------------------------
// set_ledPattern
//-----------------------------------------------------------
LOCAL void set_ledPattern(W type)
{
	if(++led_disp_row > 5) {	// 表示中のLEDの行の番号(1..5)を更新
		led_disp_row = 1;		// 行の番号が5を超えたら1に戻る
	}

	switch(type) {
		//------------------------------------------------
		// rock
		case L_ROCK:
			//tm_putstring((UB*)"LED-ROCK\n");
			set_row_gpio(led_disp_row);
			out_led_col(ledptn_rock[led_disp_row - 1]);
			break;
		// paper
		case L_PAPER:
			//tm_putstring((UB*)"LED-PAPER\n");
			set_row_gpio(led_disp_row);
			out_led_col(ledptn_paper[led_disp_row - 1]);
			break;
		// scissors
		case L_SCISSORS:
			//tm_putstring((UB*)"LED-SCISSORS\n");
			set_row_gpio(led_disp_row);
			out_led_col(ledptn_scissors[led_disp_row - 1]);
			break;

		//------------------------------------------------
		// win
		case L_WIN:
			//tm_putstring((UB*)"LED-WIN\n");
			set_row_gpio(led_disp_row);
			out_led_col(ledptn_win[led_disp_row - 1]);
			break;
		// lose
		case L_LOSE:
			//tm_putstring((UB*)"LED-LOSE\n");
			set_row_gpio(led_disp_row);
			out_led_col(ledptn_lose[led_disp_row - 1]);
			break;
		// even
		case L_EVEN:
			//tm_putstring((UB*)"LED-EVEN\n");
			set_row_gpio(led_disp_row);
			out_led_col(ledptn_even[led_disp_row - 1]);
			break;
		case L_RESET:
			//tm_putstring((UB*)"LED-RESET\n");
			set_row_gpio(led_disp_row);
			out_led_col(ledptn_reset[led_disp_row - 1]);
			break;
	}
}


//-----------------------------------------------------------
// ミリ秒単位の現在時刻の下位32ビットを返す
//-----------------------------------------------------------
LOCAL W cur_time(void)
{
	SYSTIM	tim;
	tk_get_tim(&tim);			// 現在時刻を取得
	return(tim.lo);
}


//-----------------------------------------------------------
// buttonFunction
//-----------------------------------------------------------
LOCAL void buttonFunction()
{
	UW gpio_p0in;
	BOOL btn_a, btn_b;

	// GPIO P0のINレジスタを読んでgpio_p0inに設定
	gpio_p0in = in_w(GPIO(P0, IN));

	btn_a = ((gpio_p0in & (1 << 14)) == 0); // P0.14が0の場合にTRUE
	btn_b = ((gpio_p0in & (1 << 23)) == 0); // P0.23が0の場合にTRUE

	if (btn_a)	// ボタンAの変化のチェックと送信
	{
		Bpattern = Btn_A;
		//tm_putstring((UB*)"BUTTON-A\n");
	}
	if (btn_b)	// ボタンBの変化のチェックと送信
	{
		Bpattern = Btn_B;
		//tm_putstring((UB*)"BUTTON-B\n");
	}
}


//-----------------------------------------------------------
// ボタンスイッチの入力判定と送信側タスク
//-----------------------------------------------------------
LOCAL void tx_task(INT stacd, void *exinf)
{
	for(;;) {
		// 送信する文字列を生成してtxbufに書き込む
		if (soundIndex > 0) {
			tm_sprintf((UB*) txbuf, "%d\n", soundIndex);
			uarte1_tx(1, (UB*) txbuf);

			tm_putstring((UB*)"sound:");
			tm_putstring((UB*)txbuf);

			soundIndex = S_UNKNOWN;
		}
		tk_dly_tsk(100);		// 100msのディレイ
	}
}


//-----------------------------------------------------------
// 受信したメッセージの初期化
//-----------------------------------------------------------
LOCAL void rx_clear()
{
	rx_old = 0;
	rx_current = 0;
}

//-----------------------------------------------------------
// 受信したメッセージをLEDとコンソールに表示する受信側タスク
//-----------------------------------------------------------
LOCAL void rx_task(INT stacd, void *exinf)
{
	tm_putstring((UB*)"RX task start\n");

	for (;;) {
		uarte1_rx(3, (UB*) rxbuf);

		if (Mpattern != M_Main) {
			rx_old = 0;
			rx_current = 0;

		} else {
			rxbuf[1] = '\0';
			rx_current = rxbuf[0] - '0';
			//tm_printf("cur=%d, old=%d\n", current, old);

			if (rx_current == rx_old){
				switch(rx_current) {
				case 2:
					GyankenOwner = L_ROCK;
					tm_putstring((UB*)"Player ROCK\n");
					break;
				case 1:
					GyankenOwner = L_PAPER;
					tm_putstring((UB*)"Player PAPER\n");
					break;
				case 3:
					GyankenOwner = L_SCISSORS;
					tm_putstring((UB*)"Player SCISSORS\n");
					break;
				}
			}

			rx_old = rx_current;
		}
	}
}


//-----------------------------------------------------------
// createGyanken関数
//-----------------------------------------------------------
LOCAL void createGyanken()
{
	// 手札の決定
	GyankenOwner = 0;
	UB enemy = (UB)(rand() % 3) + 1;

	switch(enemy) {
	case 1:
		GyankenEnemy = L_ROCK;
		tm_putstring((UB*)"enemy = ROCK\n");
		break;
	case 2:
		GyankenEnemy = L_PAPER;
		tm_putstring((UB*)"enemy = PAPER\n");
		break;
	case 3:
		GyankenEnemy = L_SCISSORS;
		tm_putstring((UB*)"enemy = SCISSORS\n");
		break;
	}
}


//-----------------------------------------------------------
// gameWin関数
//-----------------------------------------------------------
LOCAL void gameWin()
{
	tm_putstring((UB*)"- WIN -\n");

	// Sound
	soundIndex = S_YOU_WIN;

	// Next End
	Bpattern = Btn_U;
	Mpattern = M_End;
}


//-----------------------------------------------------------
// gameLose関数
//-----------------------------------------------------------
LOCAL void gameLose()
{
	tm_putstring((UB*)"- LOSE -\n");

	// Sound
	soundIndex = S_YOU_LOST;

	// Next End
	Bpattern = Btn_U;
	Mpattern = M_End;
}


//-----------------------------------------------------------
// gameEven関数
//-----------------------------------------------------------
LOCAL void gameEven()
{
	tm_putstring((UB*)"- EVEN -\n");

	// Sound
	soundIndex = S_AIKO;

	// LED
	set_ledPattern(GyankenEnemy);

	// 手札の決定
	createGyanken();

	// Next End
	Bpattern = Btn_U;
	Mpattern = M_Main;
}


//-----------------------------------------------------------
// mainTitle関数
//-----------------------------------------------------------
LOCAL void mainTitle()
{
	// LED
	set_ledPattern(L_EVEN);

	// ボタンを押してゲームスタート
	if (Bpattern == Btn_A) {
		Bpattern = Btn_U;

		tm_putstring((UB*)"G-Main\n");

		// Sound
		soundIndex = S_GYANKEN;

		// 手札の決定
		createGyanken();

		// To Main
		Mpattern = M_Main;
	}
}


//-----------------------------------------------------------
// mainGame関数
//-----------------------------------------------------------
LOCAL void mainGame()
{
	// ボタンを押してタイトル
	if (Bpattern == Btn_B) {
		Bpattern = Btn_U;
		Mpattern = M_Title;

		tm_putstring((UB*)"G-Title\n");

		// LED
		GyankenEnemy = L_UNKNOWN;
		set_ledPattern(L_RESET);
		return;
	}

	switch(GyankenOwner)
	{
	case L_ROCK:
		if (GyankenEnemy == L_SCISSORS) {
			gameWin();
		} else if (GyankenEnemy == L_PAPER) {
			gameLose();
		} else  {
			gameEven();
		}
		break;

	case L_PAPER:
		if (GyankenEnemy == L_ROCK) {
			gameWin();
		} else if (GyankenEnemy == L_SCISSORS) {
			gameLose();
		} else {
			gameEven();
		}
		break;

	case L_SCISSORS:
		if (GyankenEnemy == L_PAPER) {
			gameWin();
		} else if (GyankenEnemy == L_ROCK) {
			gameLose();
		} else {
			gameEven();
		}
		break;
	}
}


//-----------------------------------------------------------
// mainEnd関数
//-----------------------------------------------------------
LOCAL void mainEnd()
{
	// ボタンを押してタイトル
	if (Bpattern == Btn_B) {
		Bpattern = Btn_U;
		Mpattern = M_Title;

		tm_putstring((UB*)"G-Title\n");

		// LED
		GyankenEnemy = L_UNKNOWN;
		set_ledPattern(L_RESET);
		return;
	}

	// LED
	set_ledPattern(GyankenEnemy);
}


// タスクの生成情報
const T_CTSK ctxtsk = {0, (TA_HLNG | TA_RNG3), &tx_task, 10, 1024, 0};
const T_CTSK crxtsk = {0, (TA_HLNG | TA_RNG3), &rx_task, 10, 1024, 0};


//-----------------------------------------------------------
// usermain関数
//-----------------------------------------------------------
EXPORT INT usermain(void)
{
	tm_putstring((UB*)"Start uTKernel Serial program.\n");

	i_time = cur_time();			// システム起動時刻を保存

	btn_init();						// ボタンスイッチ入力のためのGPIOの初期設定
	led_init();						// LED制御用のGPIO設定

	init_uarte1(tx_pin, rx_pin);	// UARTEの初期化と送受信用端子の指定

	txtskid = tk_cre_tsk(&ctxtsk);	// 送信側タスクの生成
	tk_sta_tsk(txtskid, 0);			// 送信側タスクの起動

	rxtskid = tk_cre_tsk(&crxtsk);	// 受信側タスクの生成
	tk_sta_tsk(rxtskid, 0);			// 受信側タスクの起動

	tm_putstring((UB*)"G-Title\n");
	Mpattern = M_Title;

	// main loop
	for(;;)	{
		// Button
		buttonFunction();

		switch(Mpattern) {
			// title
			case M_Title:
				mainTitle();
				break;

			// main
			case M_Main:
				mainGame();
				break;

			// end
			case M_End:
				mainEnd();
				break;
		}

		tk_dly_tsk(5);
	}

	tk_slp_tsk(TMO_FEVR);

	return 0;
}
