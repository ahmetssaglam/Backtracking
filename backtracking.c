/* 
@file
BLM3021 2020-2021 GUZ ODEV-4
Bu programda verilen N boyutuna(3 <= N <= 8) göre NXN boyutunda bir renk matrisi olusturulmus ve renk bilgileri kullanicidan alinarak doldurulmustur.
Bu matrisin her bir sutununda gecen renkler 1'den fazla kez tekrar etmek zorunda mý degil mi backtracking yontemiyle kontrol edilmistir.
Ýlgili durum bilgisi ve matrisin son hali ekrana yazdirilmistir.

@author
Ýsim: Ahmet Said SAÐLAM
Öðrenci No: 17011501
Tarih: 02.01.2021
E-Mail: l1117501@std.yildiz.edu.tr
Compiler: TDM-GCC 4.9.2 64 bit-Release
IDE: DEV-C++ (version 5.11)
Ýþletim Sistemi: Windows 10 Pro 64 bit
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#include <ctype.h>

//int renk matrisi icin yer acan fonksiyon
int **colorIntAllocator(int N) {
	int **colorMatrix;	//matris icin yer acmak adina pointer to pointer
	int i;	//dongu degiskeni
	
	//calloc fonksiyonu ile dinamik olarak renk matrisinin sayisal degerlerine karsilik olacak matris icin yer acilir
	colorMatrix = (int**) calloc(N, sizeof(int*));
	for(i = 0; i < N; i++) {
		colorMatrix[i] = (int*) calloc(N, sizeof(int));
	}
	return colorMatrix;	//matris dondurulur
}


//kullanicidan renkler alinarak renk matrisi doldurulur
void getColors(int N, char colors[8][8], int **colorMatrix) {
	int i, j;	//dongu degiskenleri
	int x = 0;	//colors matrisinin satirlarina erismek icin kullanilan degisken
	char *temp = (char*) calloc(8, sizeof(char));	//kullanicidan alinacak renk icin char dizisi
	//bilgilendirme printleri
	printf("\n");
	printf("Secebileceginiz renkler : ");
	for(i = 0; i < 7; i++) {
		printf("%s, ",colors[i]);
	}
	printf("%s\n\n",colors[7]);
	//matrisin her bir gozunu gez
	for(i = 0; i < N; i++) {
		for(j = 0; j < N; j++) {
			printf("Renk Matrisinin %d. satir - %d. sutun rengini giriniz : ",i+1, j+1);
			scanf("%s",temp);	//ilgili goz icin gecerli renk degerini al
			printf("\n");
			//colorMatrix ilgili rengin int degeri ile doldurulur
			x = 0;	//0. satirdan itibaren colors'ý gez
			//renklerle karsilastir
			while(strcmp(colors[x], temp) != 0) {
				x++;
			}
			colorMatrix[i][j] = x;	//hangi renk degerine karsiliksa o rengin int degeri ile matrisin ilgili gozu doldýr
		}
	}
	free(temp);	//free islemi
}

//renk matrisini ekrana yazdiran fonksiyon
void printColors(int N, int **colorMatrix, char colors[8][8]) {
	int i, j;
//	printf("INT MATRIS : \n");
//	for(i = 0; i < N; i++) {
//		for(j = 0; j < N; j++) {
//			printf("%d\t",colorMatrix[i][j]);
//		}
//		printf("\n");
//	}
	//int renk matrisinin her bir gozunu gez
	printf("RENK TABLOSU : \n");
	for(i = 0; i < N; i++) {
		for(j = 0; j < N; j++) {
			printf("%s\t",colors[colorMatrix[i][j]]);	//o gozdeki int degere karsilik rengi yazdir
		}
		printf("\n");
	}
	
}

//verilen satirdaki elemanlari 1 kez saga shift eden fonksiyon
void shiftRight(int **colorMatrix, int N, int row_number) {
	int i;	//dongu degiskeni
	int temp;	//temporary degisken
	temp = colorMatrix[row_number][N-1];	//matrisin o satirindaki son eleman saklanir
	//matrisin ilgili satiri son elemandan basa dogru 1 kez saga kaydirilir
	for(i = N-2; i >= 0; i--) {
		colorMatrix[row_number][i+1] = colorMatrix[row_number][i];
	}
	colorMatrix[row_number][0] = temp;	//satirdaki son eleman basa donmeli. Ilk elemanin yerine yerlestirilir
}

//ust satirlardaki kolon degerleri verilen satirdaki kolon degerlerine esit degilse 1, esitlik bulunursa 0 donduren fonksiyon
int isSuitable(int **colorMatrix, int N, int row_number) {
	int i, j;
	//0. satir degil ise ust satirlara bakmak icin dongulere girilir
	if(row_number > 0 ) {
		//ilgili satirdan bir onceki satirdan itibaren
		for(i = row_number - 1; i >= 0; i--) {
			//sutunlari gez
			for(j = 0; j < N; j++) {
				//eger esitlik varsa matris uygun degildir
				if(colorMatrix[row_number][j] == colorMatrix[i][j]) {
					return 0;	//0 don
				}
			}
		}	
	}
	return 1;	//esitlige rastlanmadi ise veya zaten 0. satir kontrol ediliyorsa uygunlugu bildirmek icin 1 dondur
}

//recursive bir sekilde matrisin satirlarini sira ile gezen ve gerektiginde backtrack ile bir ust satira gecerek yeniden shift ve kontrol yapan fonksiyon
//matris uygunsa main'e 1 degil ise 0 dondurur
int backTrack(int **colorMatrix, char colors[8][8], int N, int row_number) {
	int i; //dongu degiskeni
	int control = 0;	//donus degerini kontrol eden degisken	/ 0 ile ilklendirilir
	//matrisin satirlari recursive sekilde ilerlendiginde son satir gecildi ise matris uygundur
	if(row_number >= N) {
		//printf("Matris uygun!\n");
		return 1;	//matris uygunsa 1 dondur
	}
	else {
		//sutun sayisi kadar donen for
		for(i = 0; i < N; i++) {
			//ilgili satir uygun mu diye kontrol edilir (donus degeri 1 degilse)
			if(isSuitable(colorMatrix, N, row_number) == 1 && control == 0) {
				
				//satir uygunsa bilgilendirme printi atilir(matris yazdirilir)
				if(row_number != 0) {
					printf("\n%d. satir uygun!\n",row_number + 1);
					printColors(N,colorMatrix,colors);
				}
				control = backTrack(colorMatrix, colors, N, row_number + 1);	//fonksiyon bir sonraki satira gecmek icin kendini cagirir
			}
			//satir uygun degilse ve for dongusunde satir sayisinin henuz 1 eksigi kadar donulmediyse
			if(i < N-1 && control == 0) {
				shiftRight(colorMatrix,N,row_number);	//satiri 1 kez saga shift et
			}
		}
	}
	return control;	//fonksiyon control degiskenini dondurur
}



int main() {
	
	char colors[8][8] = {
                   "kirmizi",
                   "yesil",
                   "mavi",
                   "mor",
                   "pembe",
                   "sari",
                   "turuncu",
                   "siyah"
                 };	//sistemdeki renkleri tutan matris
	
	int i;	//dongu degiskeni
	int N;	//kullanicinin girecegi renk matrisinin (NxN) boyutu
	int **colorMatrix;	//renk matrisinin sayi karsiliklarini tutacak olan matris
	
	int cont = 1;	//yeni girisler icin kullanicinin istegini kontrol eden degisken
	int control;	//recursive fonksiyondan donen degeri kontrol etmek icin degisken	
	
	//kullanýcý istedigi surece don
	while(cont) {
		
		printf("Renk matrisinin boyutunu (N) 3 ile 8 arasinda bir deger olacak sekilde giriniz : ");
		scanf("%d",&N);	//matrisin boyutu alinir
		
		colorMatrix = colorIntAllocator(N);	//renk numaralarýný tutan matris icin yer acilir

		getColors(N, colors, colorMatrix);	//renkler kullanicidan alinir
		
		printf("\n\nGirmis oldugunuz renk tablosu : \n\n");
		printColors(N, colorMatrix, colors);
		printf("\nHesaplamalar basliyor...\n\n");
		
		control = backTrack(colorMatrix, colors, N, 0);	//recursive cozum fonksiyonu cagirilir ve sonuc alinir
		//matriste her sutundaki renkler bir defa tekrar ediyorsa gecerli sonuc uretilmistir
		printf("\nHesaplamalar sonlandi...\n"); 
		if(control) {
			printf("\nMatriste her sutundaki renkler birbirlerinden farkli!\nSart saglandi! Renk Matrisi : \n\n");
			printColors(N, colorMatrix, colors);	//renk matrisi ekrana yazdirilir
		}
		//matriste her sutundaki renkler birden fazla kez tekrar ediyorsa gecerli sonuc uretilememistir
		else {
			printf("\nMatriste her sutundaki renkler birbirlerinden farkli olamiyor!\nSart saglanamadi!");
		}
		
		
		//dinamik olarak acilan yerler free edilir
		for(i = 0; i < N; i++) {
			free(colorMatrix[i]);
		}
		free(colorMatrix);
		
		printf("\n\nYeni bir renk matrisi girmek icin 1'e, cikmak icin 0'a basiniz : ");
		scanf("%d",&cont);	//kullanicidan yeni veri girisi olup olmayacaginin bilgisi alinir
		printf("\n");
	}
	
	return 0;	//end of main function
}
