
//-------- UARTEのレジスタ定義 ----------------------------------
#define	UARTE(p, r)	(UARTE##p##_BASE + UARTE_##r)

#define	UARTE0_BASE	0x40002000	// コンソール用UART0と競合するので使用しない
#define	UARTE1_BASE	0x40028000	// こちらを使用する

#define	UARTE_TASKS_STARTRX		(0x000)
#define	UARTE_TASKS_STOPRX		(0x004)
#define	UARTE_TASKS_STARTTX		(0x008)
#define	UARTE_TASKS_STOPTX		(0x00C)
#define	UARTE_TASKS_FLUSHRX		(0x02C)

#define	UARTE_EVENTS_CTS		(0x100)
#define	UARTE_EVENTS_NCTS		(0x104)
#define	UARTE_EVENTS_RXDRDY		(0x108)
#define	UARTE_EVENTS_ENDRX		(0x110)
#define	UARTE_EVENTS_TXDRDY		(0x11C)
#define	UARTE_EVENTS_ENDTX		(0x120)
#define	UARTE_EVENTS_ERROR		(0x124)
#define	UARTE_EVENTS_RXTO		(0x144)

#define	UARTE_EVENTS_RXSTARTED	(0x14C)
#define	UARTE_EVENTS_TXSTARTED	(0x150)
#define	UARTE_EVENTS_TXSTOPPED	(0x158)

#define	UARTE_SHORTS			(0x200)
#define	UARTE_INTEN				(0x300)
#define	UARTE_INTENSET			(0x304)
#define	UARTE_INTENCLR			(0x308)
#define	UARTE_ERRORSRC			(0x480)
#define	UARTE_ENABLE			(0x500)
#define	UARTE_PSEL_RTS			(0x508)
#define	UARTE_PSEL_TXD			(0x50C)
#define	UARTE_PSEL_CTS			(0x510)
#define	UARTE_PSEL_RXD			(0x514)
#define	UARTE_BAUDRATE			(0x524)

#define	UARTE_RXD_PTR			(0x534)
#define	UARTE_RXD_MAXCNT		(0x538)
#define	UARTE_RXD_AMOUNT		(0x53C)
#define	UARTE_TXD_PTR			(0x544)
#define	UARTE_TXD_MAXCNT		(0x548)
#define	UARTE_TXD_AMOUNT		(0x54C)

#define	UARTE_CONFIG			(0x56C)

//---------------------------------------------------------------
// UARTEの送信(TX)と受信(RX)に使用するエッジコネクタ端子の選択(※L)
//	以下の2行のうち一方のみコメントを外す
//#define		P0TX_P1RX	TRUE	// RING0(=P0)から送信、RING1(=P1)から受信
#define	P1TX_P0RX	TRUE	// RING1(=P1)から送信、RING0(=P0)から受信
