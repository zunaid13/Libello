// preprocessor definitions
#define EVENTLEN 150

// global varibles
/*
https://www.jagranjosh.com/general-knowledge/important-days-events-and-festivals-of-february-1549021274-1
https://www.un.org/en/observances/international-days-and-weeks
https://www.rosecalendars.co.uk/resource/notable-dates
https://www.calendarr.com/united-states/calendar-2022/
*/

// fix it for leap years!!!!
// struct arr[year][daysgone]
char special_days[367][EVENTLEN] = {
[366] = "(29/2) 29th February, only comes every 4 years :D",

[1] = "(1/1) First day of the year!!!! Also \"Global family day\"",
[4] = "(4/1) World Braille Day!",
[6] = "(6/1) World Day of War Orphans",

[35] = "(4/2) World Cancer Day",
[43] = "(12/2) Darwin Day (Darwin's birthday) also Abraham Lincoln's birthday",
[44] = "(13/2) World Radio Day",
[45] = "(14/2) Valentines Day",
[54] = "(23/2) International Mother language day",

[67] = "(8/3) Womens day",
[73] = "(14/3) Pi day!!!",
[76] = "(17/3) Birthday of Bangabandhu ALSO World children day",
[83] = "(24/3) Tuberculosis day",
[84] = "(25/3) The black night for bangalees",
[85] = "(26/3) Independence day",

[91] = "(1/4) Aprils fool day",
[104] = "(14/4) Bangla Noboborsho",

[121] = "(1/5) May day",
[127] = "(7/5) Rabindranath's Birthday",
[128] = "(8/5) Mothers day",
[144] = "(24/5) Nazrul's Birthday",

// 3rd sunday of june is fathers day
[172] = "(21/6) Longest day of the year",

[185] = "(4/7) 4th of july...special I guess?",

[219] = "(7/8) Rabindranath's death day",
[227] = "(15/8) Bangabandhu's death day",
[241] = "(29/8) Nazrul's death day",

[264] = "(21/9) World peace day",
[272] = "(29/9) World heart day",

[297] = "(24/10) United Nations day",
[304] = "(31/10) Halloweeeeeeeen ~~ mhuahahaha",

[316] = "(12/11) Pneumonia day",
[325] = "(21/11) Armed forces day",

[348] = "(14/12) Martyred Intellectuals Day",
[350] = "(16/12) Victory day",
[356] = "(22/12) Shortest day",
[358] = "(24/12) Christmas eve",
[359] = "(25/12) Christmas day",
[365] = "(31/12) Last day of the year",
};

// function prototypes
int days_gone(int m, int d);  //will return the 'th' day of the year but returns 366 for feb29
int christmas_music();

// function definitions
int days_gone(int m, int d)
{
    if(m == 2 && d == 29)
        return 366;
    int ret = 0;

    if(m > 1) ret += 31;
    if(m > 2) ret += 28;
    if(m > 3) ret += 31;
    if(m > 4) ret += 30;
    if(m > 5) ret += 31;
    if(m > 6) ret += 30;
    if(m > 7) ret += 31;
    if(m > 8) ret += 31;
    if(m > 9) ret += 30;
    if(m > 10) ret += 31;
    if(m > 11) ret += 30;

    ret += d;
    return ret;
}

int christmas_music()
{
	enum _music{
		half = 350,
		C4 = 261, D4 = 293, E4 = 329, F4 = 349, G4 = 392, A4 = 440,
		B4 = 493, C5 = 523, D5 = 587, E5 = 659, F5 = 698, G5 = 783,
		A5 = 880, B5 = 987
	};

	Beep(E5, half);
	printf("Jingle ");
	Beep(E5, half);
	printf("bell ");
	Beep(E5, half);

	Sleep(half);

	Beep(E5, half);
	printf("Jingle ");
	Beep(E5, half);
	printf("bell ");
	Beep(E5, half);

	Sleep(half);

	Beep(E5, half);
	printf("jingle ");
	Beep(G5, half);
	printf("all ");
	Beep(C5, half);
	printf("the ");
	Beep(D5, half);
	printf("way\n");
	Beep(E5, half);

	Beep(C4, half);
	Beep(D4, half);
	Beep(E4, half);

	printf("Oh ");
	Beep(F5, half);
	printf("what ");
	Beep(F5, half);
	printf("fun ");
	Beep(F5, half);

	Sleep(half);

	printf("it ");
	Beep(F5, half);
	printf("is ");
	Beep(E5, half);
	printf("to ride ");
	Beep(E5, half);

	Sleep(half);

	printf("a ");
	Beep(E5, half);
	printf("one ");
	Beep(D5, half);
	printf("horse ");
	Beep(D5, half);
	printf("open ");
	Beep(E5, half);
	printf("sleigh");
	Beep(D5, half);

	Sleep(half);

	Beep(G5, half);
}
