#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct ogretimUyesi{
	
	int ID;
	char ad[20];
	char soyad[20];
	char Unvan[20];
	
}ogretimUyesi;
typedef struct Ogrenciler{
	
	int OgrNo;
	char ad[20];
	char soyad[20];
	int toplamDers;
	int kredi;
	
}Ogrenciler;
typedef struct Dersler{
	
	int DersKodu;
	char ad[20];
	int toplamKredi;
	int kontenjan;
	int OgrID;
	
}Dersler;
typedef struct tarih{
	
	int gun,ay,yil;
	
}tarih;
typedef struct DersKayit{
	
	int ID,ogrNum,dersKodu,durum;
	tarih tarih;
}DersKayit;

void OgrenciDersKayit(int krediSiniri,int toplamDers){
	
	int tmp,dersID,kontrol=0,ogrNum,i=0,j,n,idnum=0,durum,tmp1,tmp2,kontrol2=0,toplamOgr,size;
	Ogrenciler *p;
	Dersler d;
	tarih th;
	DersKayit dk;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	
	FILE *OgrDers,*DersFile,*OgrFile;
	DersFile = fopen("Acilan_Dersler.txt","r+");
	fseek (DersFile, 0, SEEK_END); 
	size = ftell(DersFile);
	rewind(DersFile);
	
	if(size==0){
		printf("ders dosyasi bos oldugu icin kayit yapilamiyor");
		fclose(DersFile);
		return;
	}
	OgrFile = fopen("Ogrenciler.txt","r+");
	fseek (OgrFile, 0, SEEK_END); 
	size = ftell(OgrFile);
	rewind(OgrFile);
	if(size==0){
		printf("ogrenci dosyasi bos oldugu icin kayit yapilamiyor");
		fclose(OgrFile);
		return;
	}
	
	OgrDers = fopen("OgrenciDersKayit.txt","a+");
	fseek (OgrDers, 0, SEEK_END); 
	size = ftell(OgrDers);
	rewind(OgrDers);
	printf("Almak istediginiz dersin kodunu girin : ");
	
	while(kontrol==0 && kontrol2==0){
		
		scanf("%d",&dersID);
		rewind(DersFile);
		rewind(OgrDers);
		kontrol=0;
		kontrol2=0;
		toplamOgr=0;
		
		while(kontrol==0 && !feof(DersFile)){
			fscanf(DersFile,"%d %s %d %d %d\n",&d.DersKodu,d.ad,&d.toplamKredi,&d.kontenjan,&d.OgrID);
			if(dersID==d.DersKodu) kontrol++;
			
		}
		if(kontrol==0) printf("boyle bir ders bulunamadi, ders kodunu tekrar girin: ");
		else{
			
			while(!feof(OgrDers)){
				fscanf(OgrDers,"%d %d %d %d    %d %d %d\n",&dk.ID,&dk.ogrNum,&dk.dersKodu,&dk.durum,&dk.tarih.gun,&dk.tarih.ay,&dk.tarih.yil);
				if(dk.dersKodu==dersID && dk.durum==1) toplamOgr++;
			}
			if(toplamOgr>=d.kontenjan){
				printf(" Kontenjan dolu oldugundan bu derse kayit yapilamaz.");
				fclose(OgrDers);
				fclose(DersFile);
				return;	
			} 
			else kontrol2++;
		}
	}
	kontrol=0;
	kontrol2=0;
	printf("Dersi alacak ogrencinin numarasini girin: ");
	while(kontrol==0 && kontrol2==0){
		scanf("%d",&ogrNum);
		i=0;
		p = (Ogrenciler*)malloc(sizeof(Ogrenciler));
		rewind(OgrFile);
		kontrol=0;
		kontrol2=0;
		while(!feof(OgrFile)){
			
			p = (Ogrenciler*)realloc(p,sizeof(Ogrenciler)*(i+1));
			fscanf(OgrFile,"%d %s %s %d %d\n",&p[i].OgrNo,p[i].ad,p[i].soyad,&p[i].toplamDers,&p[i].kredi);
			
			if(p[i].OgrNo==ogrNum){
				kontrol++;
				j=i;
			} 
			i++;
		}
		if(kontrol==0) {
			printf("Bu numarada bir ogrenci yok. Numarayi tekrar girin: ");
			free(p);	
		}
		else{
			
			if(krediSiniri<p[j].kredi+d.toplamKredi){
				printf("kredi siniri asimi oldugundan bu ogrenci bu dersi alamaz.");
				fclose(OgrDers);
				fclose(DersFile);
				return;
			}
			else if(toplamDers<p[j].toplamDers+1){
				printf("maksimum ders sayisina ulasilacagindan bu guncelleme yapilamaz.");
				fclose(OgrDers);
				fclose(DersFile);
				return;
			}
			else{
				kontrol2++;
			}
		}
	}
	
	fclose(OgrFile);
	while(!feof(OgrDers)){
		
		fscanf(OgrDers,"%d %d %d %d    %d %d %d\n",&dk.ID,&dk.ogrNum,&dk.dersKodu,&dk.durum,&dk.tarih.gun,&dk.tarih.ay,&dk.tarih.yil);
		if(dk.ogrNum==ogrNum && dk.dersKodu==dersID){
			printf("bu ogrenci zaten OgrenciDersKayit dosyasinda var. Guncellemeyi deneyin.");
			fclose(DersFile);
			fclose(OgrDers);
			return;	
		} 
	}
	n=i;
	if(kontrol!=0){
		OgrFile = fopen("Ogrenciler.txt","w+");
		
		for(i=0;i<n;i++){
			
			if(i!=j) fprintf(OgrFile,"%d %s %s %d %d\n",p[i].OgrNo,p[i].ad,p[i].soyad,p[i].toplamDers,p[i].kredi);
			else{
				p[j].toplamDers++;
				p[j].kredi = p[j].kredi + d.toplamKredi;
				fprintf(OgrFile,"%d %s %s %d %d\n",p[i].OgrNo,p[i].ad,p[i].soyad,p[i].toplamDers,p[i].kredi);
			}
		}
	}
	th.gun=tm.tm_mday;
	th.ay=tm.tm_mon+1;
	th.yil=tm.tm_year+1900;
	rewind(OgrDers);
	dk.ID++;
	idnum=dk.ID;
	if(size==0){
		idnum=0; 
	}
	fprintf(OgrDers,"%d %d %d 1    %d %d %d\n",idnum,ogrNum,dersID,th.gun,th.ay,th.yil);
	
	
	fclose(OgrFile);
	fclose(DersFile);
	fclose(OgrDers);
	free(p);
}

void OgrenciDersKayitSil(Dersler ders){
	
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	FILE *ogrDers,*OgrFile;
	int i=0,ogrNum,j=0,n,kontrol=0,m,*arr,size;
	DersKayit *p;
	Ogrenciler *o;
	ogrDers = fopen("OgrenciDersKayit.txt","r+");
	OgrFile = fopen("Ogrenciler.txt","r+");
	p = (DersKayit*)malloc(sizeof(DersKayit));
	o = (Ogrenciler*)malloc(sizeof(Ogrenciler));
	arr = (int*)malloc(sizeof(int));
	while(!feof(ogrDers)){
		
		p = (DersKayit*)realloc(p,sizeof(DersKayit)*(i+1));
		fscanf(ogrDers,"%d %d %d %d   %d %d %d\n",&p[i].ID,&p[i].ogrNum,&p[i].dersKodu,&p[i].durum,&p[i].tarih.gun,&p[i].tarih.ay,&p[i].tarih.yil);
		
		if(ders.DersKodu==p[i].dersKodu && p[i].durum==1){
			
			p[i].durum=0;
			arr = (int*)realloc(arr,sizeof(int)*(j+1));
			arr[j]=p[i].ogrNum;
			j++;
			
		} 
		i++;
	}
	n=i;
	m=j;
	fclose(ogrDers);
	ogrDers = fopen("OgrenciDersKayit.txt","w+");
	for(i=0;i<n;i++) fprintf(ogrDers,"%d %d %d %d   %d %d %d\n",p[i].ID,p[i].ogrNum,p[i].dersKodu,p[i].durum,p[i].tarih.gun,p[i].tarih.ay,p[i].tarih.yil);
	
	i=0;
	while(!feof(OgrFile)){
		
		o = (Ogrenciler*)realloc(o,sizeof(Ogrenciler)*(i+1));
		fscanf(OgrFile,"%d  %s  %s  %d  %d\n",&o[i].OgrNo,o[i].ad,o[i].soyad,&o[i].toplamDers,&o[i].kredi);
		
		for(j=0;j<m;j++){
			
			if(arr[j]==o[i].OgrNo){
				
				o[i].toplamDers--;
				o[i].kredi=o[i].kredi-ders.toplamKredi;
				
			}
		}
		i++;
	}
	fclose(OgrFile);
	m=i;
	OgrFile = fopen("Ogrenciler.txt","w+");
	for(i=0;i<m;i++) fprintf(OgrFile,"%d  %s  %s  %d  %d\n",o[i].OgrNo,o[i].ad,o[i].soyad,o[i].toplamDers,o[i].kredi);
	fclose(OgrFile);
	fclose(ogrDers);
	free(p);
	free(o);
}
void OgrenciDersKayitGuncelle(int OgrID,int dersKodu){
	
	FILE *OgrDers,*OgrFile,*DersFile;
	DersKayit *p;
	Ogrenciler *d;
	Dersler x;
	int i=0,n,size,j=0,kontrol=0;
	DersFile = fopen("Acilan_Dersler.txt","r+");
	OgrDers = fopen("OgrenciDersKayit.txt","r+");
	OgrFile = fopen("Ogrenciler.txt","r+");
	fseek (OgrDers, 0, SEEK_END);
	size = ftell(OgrDers);
	rewind(OgrDers);
	if(size==0){
		printf("kayit dosyasi bos oldugundan islem yapilamaz.");
		fclose(DersFile);
		fclose(OgrFile);
		fclose(OgrDers);
		return;
	}
	fseek (DersFile, 0, SEEK_END);
	size = ftell(DersFile);
	rewind(DersFile);
	if(size==0){
		printf("ders dosyasi bos oldugundan islem yapilamaz.");
		fclose(DersFile);
		fclose(OgrFile);
		fclose(OgrDers);
		return;
	}
	fseek (OgrFile, 0, SEEK_END);
	size = ftell(OgrFile);
	rewind(OgrFile);
	if(size==0){
		printf("ogrenci dosyasi bos oldugundan islem yapilamaz.");
		fclose(DersFile);
		fclose(OgrFile);
		fclose(OgrDers);
		return;
	}
	if(dersKodu!=-1){
		while(kontrol==0 && !feof(DersFile)){
			fscanf(DersFile,"%d %s %d %d %d\n",&x.DersKodu,x.ad,&x.toplamKredi,&x.kontenjan,&x.OgrID);
			if(x.DersKodu==dersKodu) kontrol++;
		}
		if(kontrol==0){
			printf("boyle bir ders yok.Islem yapilamadi ");
			fclose(DersFile);
			fclose(OgrFile);
			fclose(OgrDers);
			return;
		}	
	}
	fclose(DersFile);
	p = (DersKayit*)malloc(sizeof(DersKayit));
	d = (Ogrenciler*)malloc(sizeof(Ogrenciler));
	kontrol=0;
	while(!feof(OgrDers)){
		rewind(OgrFile);
		p = (DersKayit*)realloc(p,sizeof(DersKayit)*(i+1));
		fscanf(OgrDers,"%d %d %d %d   %d %d %d\n",&p[i].ID,&p[i].ogrNum,&p[i].dersKodu,&p[i].durum,&p[i].tarih.gun,&p[i].tarih.ay,&p[i].tarih.yil);
		if(p[i].ogrNum==OgrID && p[i].durum==1 && dersKodu==-1) {
			p[i].durum=0;		
			
		}
		if(p[i].ogrNum==OgrID && p[i].dersKodu==dersKodu){
			
			if(p[i].durum==1){
				p[i].durum=0;
				while(!feof(OgrFile)){
					d = (Ogrenciler*)realloc(d,sizeof(Ogrenciler)*(j+1));
					fscanf(OgrFile,"%d  %s  %s  %d  %d\n",&d[j].OgrNo,d[j].ad,d[j].soyad,&d[j].toplamDers,&d[j].kredi);
					if(OgrID==d[j].OgrNo){
						d[j].toplamDers--;
						d[j].kredi = d[j].kredi-x.toplamKredi;
					}
					j++;
				}	
			}
			else {
				p[i].durum=1;
				while(!feof(OgrFile)){
					d = (Ogrenciler*)realloc(d,sizeof(Ogrenciler)*(j+1));
					fscanf(OgrFile,"%d  %s  %s  %d  %d\n",&d[j].OgrNo,d[j].ad,d[j].soyad,&d[j].toplamDers,&d[j].kredi);
					if(OgrID==d[j].OgrNo){
						d[j].toplamDers++;
						d[j].kredi = d[j].kredi+x.toplamKredi;
					}
					j++;
				}	
			}
			kontrol++;
		}
		i++;
	}
	fclose(OgrFile);
	fclose(OgrDers);
	if(kontrol==0){
			printf("OgrenciDersKayit dosyasinda boyle bir eslesme yok.");
			return;
		}
	n=i;
	OgrDers = fopen("OgrenciDersKayit.txt","w+");
	OgrFile = fopen("Ogrenciler.txt","w+");
	
	for(i=0;i<n;i++) fprintf(OgrDers,"%d %d %d %d   %d %d %d\n",p[i].ID,p[i].ogrNum,p[i].dersKodu,p[i].durum,p[i].tarih.gun,p[i].tarih.ay,p[i].tarih.yil);
	n=j;
	for(i=0;i<n;i++) fprintf(OgrFile,"%d  %s  %s  %d  %d\n",d[i].OgrNo,d[i].ad,d[i].soyad,d[i].toplamDers,d[i].kredi);
	fclose(OgrFile);
	fclose(OgrDers);
	free(p);
	free(d);
	printf("kayit guncellendi.");
}

void OgrenciEkle(){
	
	int kontrol=0,kontrol2,OgrID,kontrol3,size;
	Ogrenciler p;
	FILE *Ogrfile;
	
	Ogrfile=fopen("Ogrenciler.txt","a+");
	
	printf("Ogrencinin bilgilerini girin: ");
	printf("\nOgrencinin numarasi: ");
	
	fseek (Ogrfile, 0, SEEK_END); 
	size = ftell(Ogrfile);
	rewind(Ogrfile);
	while(kontrol==0){
		
		rewind(Ogrfile);
		scanf("%d",&OgrID);
		kontrol2=0;
		kontrol3=0;
		while(!feof(Ogrfile) && kontrol2==0 && kontrol3==0){
			fscanf(Ogrfile,"%d  %s  %s  %d  %d\n",&p.OgrNo,p.ad,p.soyad,&p.toplamDers,&p.kredi);
			if(size!=0){
				if(p.OgrNo==OgrID) kontrol2++;
				if(OgrID<0) kontrol3++;			
			}
		}
		if(kontrol2!=0) printf("Bu numarada bir Ogrenci zaten var. Farkli bir numara girin. ");
		else if(kontrol3!=0) printf("Ogrenci numarasi negatif bir deger olamaz. Tekrar girin. ");
		else kontrol++;
	}
		printf("Ogrencinin adi: ");
		scanf("%s",p.ad);
	
		printf("ogrencinin soyadi: ");
		scanf("%s",p.soyad);
	
		fprintf(Ogrfile,"%d  %s  %s  0  0\n",OgrID,p.ad,p.soyad);	
	
	fclose(Ogrfile);
}
void OgrenciSil(int ogrID){
	
	FILE *ogrFile;
	Ogrenciler *p;
	int tmp,OgrID,kontrol=0,i=0,n,size;
	ogrFile = fopen("Ogrenciler.txt","r+");
	fseek (ogrFile, 0, SEEK_END); 
	size = ftell(ogrFile);
	rewind(ogrFile);
	if(size==0){
		printf("ogrenci dosyasi bos oldugundan islem yapilamaz.");
		fclose(ogrFile);
		return;
	}
	p = (Ogrenciler*)malloc(sizeof(Ogrenciler));
	
	while(!feof(ogrFile)){
		
		p = (Ogrenciler*)realloc(p,sizeof(Ogrenciler)*(i+1));
		fscanf(ogrFile,"%d %s %s %d %d\n",&p[i].OgrNo,p[i].ad,p[i].soyad,&p[i].toplamDers,&p[i].kredi);
		
		if(ogrID!=p[i].OgrNo) i++;
		else {
			kontrol++;	
			OgrenciDersKayitGuncelle(p[i].OgrNo,-1);
		}
	}
	fclose(ogrFile);
	if(kontrol==0){
		printf("Bu numarada bir ogrenci yok.");
	}
	else{
		ogrFile = fopen("Ogrenciler.txt","w+");
		n=i;
		
		for(i=0;i<n;i++) fprintf(ogrFile,"%d %s %s %d %d\n",p[i].OgrNo,p[i].ad,p[i].soyad,p[i].toplamDers,p[i].kredi);
		fclose(ogrFile);
		free(p);
	}
}
void OgrenciGuncelle(){
	
		Ogrenciler *p;
		int i=0,OgrID,n,kontrol=0,size;
		printf("Guncellemek istediginiz ogrencinin numarasini girin: ");
		scanf("%d",&OgrID);
		FILE *OgrFile;
		OgrFile = fopen("Ogrenciler.txt","r+");
		fseek (OgrFile, 0, SEEK_END); 
		size = ftell(OgrFile);
		rewind(OgrFile);
		if(size==0){
			printf("ogrenci dosyasi bos oldugundan islem yapilamaz.");
			fclose(OgrFile);
			return;
		}
		p = (Ogrenciler*)malloc(sizeof(Ogrenciler));
		while(!feof(OgrFile)){
			
			p = (Ogrenciler*)realloc(p,sizeof(Ogrenciler)*(i+1));
			fscanf(OgrFile,"%d %s %s %d %d\n",&p[i].OgrNo,p[i].ad,p[i].soyad,&p[i].toplamDers,&p[i].kredi);
			if(p[i].OgrNo==OgrID){
				
				printf("Girdiginiz numaraya sahip Ogrencinin yeni bilgilerini girin: ");
				printf("\nAd: ");
				scanf("%s",p[i].ad);
	
				printf("soyad: ");
				scanf("%s",p[i].soyad);
				kontrol++;
				
			}
			i++;
		}
		n=i;
		fclose(OgrFile);
		if(kontrol==0) printf("bu numarada bir ogrenci bulunamadi. ");
		else{
			
			OgrFile = fopen("Ogrenciler.txt","w+");
			for(i=0;i<n;i++) fprintf(OgrFile,"%d %s %s %d %d\n",p[i].OgrNo,p[i].ad,p[i].soyad,p[i].toplamDers,p[i].kredi);
		
		fclose(OgrFile);
		}
		free(p);
}

void OgretimUyesiEkle(){
	
	FILE *OgrFile;
	ogretimUyesi ou;
	OgrFile=fopen("OgretimUyeleri.txt","a+");
	int tmp,kontrol=0,kontrol2=0,OgrID,kontrol3,size;
	printf("\nOgretim Uyesinin Bilgilerini Girin: ");
	
	fseek (OgrFile, 0, SEEK_END); 
	size = ftell(OgrFile);
	rewind(OgrFile);
	printf("\nID: ");
	
	while(kontrol==0){
		
		rewind(OgrFile);	
		scanf("%d",&OgrID);
		kontrol2=0;
		kontrol3=0;
		while(!feof(OgrFile) && kontrol2==0 && kontrol3==0){
			
			fscanf(OgrFile,"%d  %s  %s  %s\n",&ou.ID,ou.ad,ou.soyad,ou.Unvan);
			if(size!=0){
				if(ou.ID==OgrID){
					printf("Bu eleman zaten var. Yanlis girmis olmalisiniz, tekrar girin: ");
					kontrol2++;
				}
				if(OgrID<0){
					printf("Ogretmen IDsi negatif bir deger olamaz. Tekrar girin. ");
					kontrol3++;
				}	
			}
		}
		if(kontrol2==0 && kontrol3==0) kontrol++;	
	}
	
	printf("Ad: ");
	scanf("%s",ou.ad);
	
	printf("soyad: ");
	scanf("%s",ou.soyad);
	
	printf("Unvan: ");
	scanf("%s",ou.Unvan);
	
	fprintf(OgrFile,"%d  %s  %s  %s\n",OgrID,ou.ad,ou.soyad,ou.Unvan);
	
	fclose(OgrFile);
}

void OgretimUyesiSil(int OgrID){
	
	FILE *OgrFile;
	OgrFile = fopen("OgretimUyeleri.txt","r+");
	int i=0,tmp,n,kontrol=0,size;
	fseek (OgrFile, 0, SEEK_END); 
	size = ftell(OgrFile);
	rewind(OgrFile);
	if(size==0){
		printf("Ogretim Uyesi dosyasi bos oldugundan islem yapilamaz.");
		fclose(OgrFile);
		return;
	}
	ogretimUyesi *p;
	p = (ogretimUyesi*)malloc(sizeof(ogretimUyesi));
	i=0;
	while(!feof(OgrFile)){
		
			p = (ogretimUyesi*)realloc(p,sizeof(ogretimUyesi)*(i+1));
		
			fscanf(OgrFile,"%d %s %s %s\n",&tmp,p[i].ad,p[i].soyad,p[i].Unvan);
			p[i].ID = tmp;
		
		
			if(OgrID!=p[i].ID) 	i++;
			else kontrol++;
		}
	fclose(OgrFile);
	Dersler d;
	if(kontrol==0) 	printf("Bu eleman listede yok.");
	else{
		FILE *dersFile;
		OgrFile = fopen("OgretimUyeleri.txt","w+");
		dersFile = fopen("Acilan_Dersler.txt","r+");
		n=i;
		kontrol=0;
		for(i=0;i<n;i++) fprintf(OgrFile,"%d %s %s %s\n",p[i].ID,p[i].ad,p[i].soyad,p[i].Unvan);		
		
	
		while(kontrol==0 && !feof(dersFile)){
			
			fscanf(dersFile,"%d %s %d %d %d\n",&d.DersKodu,d.ad,&d.toplamKredi,&d.kontenjan,&d.OgrID);
			if(OgrID==d.OgrID) kontrol++;
			
		}
		fclose(OgrFile);
		fclose(dersFile);
		printf("Ogretim Uyesi silindi.");
		if(kontrol!=0){
			
			DersSil(d.DersKodu);
			
		}	
	}
	free(p);
}
void OgretimUyesiGuncelle(){
	
		FILE *OgrFile;
		int OgrID,i=0,n,kontrol=0,size;
		printf("islem yapmak istediginiz Ogretim uyesinin ID numarasini girin :");
		scanf("%d",&OgrID);
		ogretimUyesi *p;
		
		OgrFile = fopen("OgretimUyeleri.txt","r+");
		fseek (OgrFile, 0, SEEK_END); 
		size = ftell(OgrFile);
		rewind(OgrFile);
		if(size==0){
			printf("Ogretim Uyesi dosyasi bos oldugundan islem yapilamaz.");
			fclose(OgrFile);
			return;
		}
		p = (ogretimUyesi*)malloc(sizeof(ogretimUyesi));
		while(!feof(OgrFile)){
			
			p = (ogretimUyesi*)realloc(p,sizeof(ogretimUyesi)*(i+1));
			fscanf(OgrFile,"%d  %s  %s  %s\n",&p[i].ID,p[i].ad,p[i].soyad,p[i].Unvan);
			if(p[i].ID==OgrID){
				
				kontrol++;
				printf("Girdiginiz ID'ye sahip ogretim Uyesinin yeni bilgilerini girin: ");
				printf("\nAd: ");
				scanf("%s",p[i].ad);
	
				printf("soyad: ");
				scanf("%s",p[i].soyad);
	
				printf("Unvan: ");
				scanf("%s",p[i].Unvan);
				
			}
			i++;
		}
		fclose(OgrFile);
		if(kontrol==0) printf("boyle bir ogretim uyesi yok.");
		else{
			
			OgrFile = fopen("OgretimUyeleri.txt","w+");
			n=i;
			for(i=0;i<n;i++){
				
				fprintf(OgrFile,"%d  %s  %s  %s\n",p[i].ID,p[i].ad,p[i].soyad,p[i].Unvan);
				
			}
		}
		fclose(OgrFile);
		free(p);
}

void DersEkle(){
	
	FILE *dersfile,*ogretmenFile;
	Dersler p;
	ogretimUyesi o;
	int kontrol=0,dersKodu,kontrol2=0,kontrol3=0,size;
	
	dersfile=fopen("Acilan_Dersler.txt","a+");
	ogretmenFile=fopen("OgretimUyeleri.txt","r+");

	printf("Ders bilgilerini girin ");
	printf("\nDers kodunu girin (sadece sayi) :");
	
	fseek (dersfile, 0, SEEK_END); 
	size = ftell(dersfile);
	rewind(dersfile);
	while(kontrol==0){
		
		rewind(dersfile);
		scanf("%d",&dersKodu);
		kontrol2=0;
		kontrol3=0;
		while(!feof(dersfile) && kontrol2==0 && kontrol3==0){
			fscanf(dersfile,"%d  %s  %d  %d  %d\n",&p.DersKodu,p.ad,&p.toplamKredi,&p.kontenjan,&p.OgrID);
			if(size!=0){
				if(p.DersKodu==dersKodu) kontrol2++;
				if(dersKodu<0) kontrol3++;	
			}
		}
		if(kontrol2!=0) printf("Bu kodda bir ders zaten var. Farkli bir kod girin. ");	
		else if(kontrol3!=0) printf("Ders kodu negatif bir deðer olamaz. Farkli bir kod girin."); 
		else kontrol++;
	}
	
	printf("Dersin adini girin: ");
	scanf("%s",p.ad);
	
	printf("Dersin toplam kredisini girin: ");
	scanf("%d",&p.toplamKredi);
	
	printf("Ders kontenjanini girin: ");
	scanf("%d",&p.kontenjan);
	
	printf("Dersi veren ogretmenin ID'sini girin: ");
	kontrol=0;
	fseek (ogretmenFile, 0, SEEK_END); 
	size = ftell(ogretmenFile);
	rewind(ogretmenFile);
	while(kontrol==0){
			
		rewind(ogretmenFile);
		scanf("%d",&p.OgrID);
		while(!feof(ogretmenFile) && kontrol==0){
			
			fscanf(ogretmenFile,"%d	 %s  %s  %s\n",&o.ID,o.ad,o.soyad,o.Unvan);
			if(size==0){		
				printf("Ogretim uyesi dosyasý bos oldugundan ogretim uyesi secilemiyor.");
				fclose(ogretmenFile);
				fclose(dersfile);
				return;
			}
			if(o.ID==p.OgrID) kontrol++;
			
		}
		if(kontrol==0) printf("Bu ID herhangi bir ogretmenle eslesmiyor. tekrar girin: ");
	}
	fprintf(dersfile,"%d  %s  %d  %d  %d\n",dersKodu,p.ad,p.toplamKredi,p.kontenjan,p.OgrID);
	fclose(dersfile);
	fclose(ogretmenFile);
	
}

void DersSil(int dersKodu){
	
	FILE *dersFile;
	int tmp,n,i=0,kontrol=0,size;
	dersFile = fopen ("Acilan_Dersler.txt","r+");
	fseek (dersFile, 0, SEEK_END); 
	size = ftell(dersFile);
	rewind(dersFile);
	if(size==0){
		printf("ders dosyasi bos oldugundan islem yapilamaz.");
		fclose(dersFile);
		return;
	}
	Dersler *p;
	
	p = (Dersler*)malloc(sizeof(Dersler));
	
	while(!feof(dersFile)){
		
		p = (Dersler*)realloc(p,sizeof(Dersler)*(i+1));
		
		fscanf(dersFile,"%d %s %d %d %d\n",&p[i].DersKodu,p[i].ad,&p[i].toplamKredi,&p[i].kontenjan,&p[i].OgrID);
		
		if(p[i].DersKodu!=dersKodu) i++;
		else {
			
			OgrenciDersKayitSil(p[i]);
			kontrol++;	
		}
		
	}
	
	fclose(dersFile);
	if(kontrol==0) printf("Bu kodda bir ders zaten bulunmuyor.");
	else{
		dersFile = fopen("Acilan_Dersler.txt","w+");
		n=i;
		for(i=0;i<n;i++) fprintf(dersFile,"%d %s %d %d %d\n",p[i].DersKodu,p[i].ad,p[i].toplamKredi,p[i].kontenjan,p[i].OgrID);
		fclose(dersFile);
		printf("\n ders silindi.");	
	}
	free(p);
}

void DersGuncelle(int krediSiniri){
	
	FILE *DersFile,*OgrDers,*OgrFile;
	Dersler *p,p2;
	DersKayit *d;
	Ogrenciler *o;
	int i=0,n,dersKodu,j=0,k=0,eskiKredi,tmpKredi,eskiOgrKredi,m,m2,eskiKontenjan,toplamOgrenci=0,kontrol=0,size;
	printf("Guncellemek istediginiz dersin kodunu giriniz: ");
	scanf("%d",&dersKodu);
	kontrol=0;
	DersFile = fopen("Acilan_Dersler.txt","r+");
	while(!feof(DersFile)){
		
		fscanf(DersFile,"%d %s %d %d %d\n",&p2.DersKodu,p2.ad,&p2.toplamKredi,&p2.kontenjan,&p2.OgrID);
		if(p2.DersKodu==dersKodu) kontrol++;
		
	}
	rewind(DersFile);
	if(kontrol==0){
		printf("bu kod bir ders koduyla eslesmiyor.");
		fclose(DersFile);
		return;
	}
	fseek (DersFile, 0, SEEK_END); 
	size = ftell(DersFile);
	rewind(DersFile);
	if(size==0){
		printf("ders dosyasi bos oldugundan islem yapilamaz.");
		fclose(DersFile);
		return;
	}
	OgrDers = fopen("OgrenciDersKayit.txt","r+");
	OgrFile = fopen("Ogrenciler.txt","r+");
	p = (Dersler*)malloc(sizeof(Dersler));
	d = (DersKayit*)malloc(sizeof(DersKayit));
	o = (Ogrenciler*)malloc(sizeof(Ogrenciler));
	
	while(!feof(OgrFile)){
						
		o = (Ogrenciler*)realloc(o,sizeof(Ogrenciler)*(k+1));
		fscanf(OgrFile,"%d  %s  %s  %d  %d\n",&o[k].OgrNo,o[k].ad,o[k].soyad,&o[k].toplamDers,&o[k].kredi);
		k++;
		
	}
	n=k;
	k=0;
	while(!feof(DersFile)){
		
		p = (Dersler*)realloc(p,sizeof(Dersler)*(i+1));
		fscanf(DersFile,"%d %s %d %d %d\n",&p[i].DersKodu,p[i].ad,&p[i].toplamKredi,&p[i].kontenjan,&p[i].OgrID);
		if(dersKodu==p[i].DersKodu){
			eskiKredi = p[i].toplamKredi;
			printf("Dersin toplam kredisini girin: ");  //kontrol
			scanf("%d",&p[i].toplamKredi);
			while(!feof(OgrDers)){
				
				d = (DersKayit*)realloc(d,sizeof(DersKayit)*(j+1));
				fscanf(OgrDers,"%d %d %d %d   %d %d %d\n",&d[j].ID,&d[j].ogrNum,&d[j].dersKodu,&d[j].durum,&d[j].tarih.gun,&d[j].tarih.ay,&d[j].tarih.yil);
				if(d[j].dersKodu == p[i].DersKodu && d[j].durum==1){
					toplamOgrenci++;
					for(k=0;k<n;k++){
						
						if(d[j].ogrNum==o[k].OgrNo) {
							
							eskiOgrKredi = o[k].kredi;
							tmpKredi = eskiKredi - p[i].toplamKredi;
							eskiOgrKredi = eskiOgrKredi-tmpKredi;
							
							if(eskiOgrKredi>krediSiniri){
								
								printf("kredi siniri tasan ogrenciler oldugundan bu ders bu sekilde guncellenemez. ");	
								fclose(OgrDers);
								fclose(DersFile);
								fclose(OgrFile);
								return;
							}
							else o[k].kredi=eskiOgrKredi;		
						}
					}
				}
				j++;
			}
			m2=j;
			printf("Ders kontenjanini girin: ");
			eskiKontenjan = p[i].kontenjan;
			scanf("%d",&p[i].kontenjan);
			if(toplamOgrenci>p[i].kontenjan){
				toplamOgrenci=toplamOgrenci-p[i].kontenjan;
				printf("Kontenjan asimi oldugundan bu sekilde guncellenemedi. Once %d kadar ogrenci silmelisiniz.",toplamOgrenci);
				p[i].kontenjan=eskiKontenjan;
				fclose(OgrFile);
				fclose(DersFile);
				fclose(OgrDers);
				return;
			}
			
		printf("Dersi veren ogretmenin ID'sini girin: ");
		kontrol=0;
		while(kontrol==0){
			FILE *ogr;
			ogretimUyesi x;
			ogr = fopen("OgretimUyeleri.txt","r+");
			scanf("%d",&m);
			while(!feof(ogr)){
				
				fscanf(ogr,"%d	 %s  %s  %s\n",&x.ID,x.ad,x.soyad,x.Unvan);
				if(x.ID==m) {
					p[i].OgrID=x.ID;
					kontrol++;	
					}
				}
			if(kontrol==0) printf("bu ID bir ogretim uyesi ile eslesmedi. tekrar girin.");
			}
		}
		i++;
	}m=i;
	fclose(DersFile);
	fclose(OgrDers);
	fclose(OgrFile);
	
	OgrFile = fopen("Ogrenciler.txt","w+");
	for(i=0;i<n;i++) fprintf(OgrFile,"%d %s %s %d %d\n",o[i].OgrNo,o[i].ad,o[i].soyad,o[i].toplamDers,o[i].kredi);
	fclose(OgrFile);
	OgrDers = fopen("OgrenciDersKayit.txt","w+");
	for(i=0;i<m2;i++) fprintf(OgrDers,"%d %d %d %d   %d %d %d\n",d[i].ID,d[i].ogrNum,d[i].dersKodu,d[i].durum,d[i].tarih.gun,d[i].tarih.ay,d[i].tarih.yil);
	fclose(OgrDers);
	DersFile = fopen("Acilan_Dersler.txt","w+");
	for(i=0;i<m;i++) fprintf(DersFile,"%d %s %d %d %d\n",p[i].DersKodu,p[i].ad,p[i].toplamKredi,p[i].kontenjan,p[i].OgrID);
	fclose(DersFile);
	free(p);
	free(d);
	free(o);
	
}

Dersler* Ogretim_Uyesine_Gore_Ders_Listele(int *n){
	
	FILE *dersFile,*OgrFile;
	int OgrID,i=0,kontrol=0;
	Dersler p,*p2;
	ogretimUyesi o;
	p2 = (Dersler*)malloc(sizeof(Dersler));
	printf("hangi Ogretmen ID sinin verdigi dersleri goruntulemek istiyorsunuz: ");
	scanf("%d",&OgrID);
	dersFile = fopen("Acilan_Dersler.txt","r+");
	OgrFile = fopen("OgretimUyeleri.txt","r+");
	
	while(kontrol==0 && !feof(OgrFile)){
		
		fscanf(OgrFile,"%d  %s  %s  %s\n",&o.ID,o.ad,o.soyad,o.Unvan);
		if(OgrID==o.ID) kontrol++;
		
	}
	if(kontrol==0){
		printf("boyle bir ogretmen yok.");
		fclose(dersFile);
		fclose(OgrFile);
		p2[0].DersKodu=-1;
		return p2;
	}
	printf("%s %s %s  isimli ogretim uyesinin verdigi dersler: ",o.Unvan,o.ad,o.soyad);
	while(!feof(dersFile)){
		
		fscanf(dersFile,"%d  %s  %d  %d  %d\n",&p.DersKodu,p.ad,&p.toplamKredi,&p.kontenjan,&p.OgrID);
		if(p.OgrID==OgrID){
			p2 = (Dersler*)realloc(p2,sizeof(Dersler)*(i+1));
			p2[i]=p;
			printf("\n%d  %s",p.DersKodu,p.ad);
			i++;
		}
	}
	(*n)=i;
	fclose(OgrFile);
	fclose(dersFile);
	return p2;
}
void Ders_Koduna_Gore_Ogrenci_Listele(){
	
	FILE *OgrDers,*dersFile,*OgrFile;
	dersFile = fopen("Acilan_Dersler.txt","r+");
	OgrDers = fopen("OgrenciDersKayit.txt","r+");
	OgrFile = fopen("Ogrenciler.txt","r+");
	int dersKodu,kontrol=0;
	Dersler p;
	DersKayit d;
	Ogrenciler o;
	
	printf("Ders kodunu girin: ");
	scanf("%d",&dersKodu);
	while(kontrol==0 && !feof(dersFile)){
		
		fscanf(dersFile,"%d  %s  %d  %d  %d\n",&p.DersKodu,p.ad,&p.toplamKredi,&p.kontenjan,&p.OgrID);
		if(dersKodu==p.DersKodu) kontrol++;
		
	}
	if(kontrol==0){
		printf("boyle bir ders yok.");
		fclose(OgrDers);
		fclose(dersFile);
		fclose(OgrFile);
		return;
	}
	while(!feof(OgrDers)){
		
		fscanf(OgrDers,"%d %d %d %d   %d %d %d\n",&d.ID,&d.ogrNum,&d.dersKodu,&d.durum,&d.tarih.gun,&d.tarih.ay,&d.tarih.yil);
		if(d.dersKodu==dersKodu && d.durum==1){
			rewind(OgrFile);
			while(!feof(OgrFile)){
				
				fscanf(OgrFile,"%d  %s  %s  %d  %d\n",&o.OgrNo,o.ad,o.soyad,&o.toplamDers,&o.kredi);
				if(o.OgrNo==d.ogrNum){
					
					printf("\n%d %s %s",o.OgrNo,o.ad,o.soyad);
					
				}
			}
		}
	}
	fclose(OgrDers);
	fclose(dersFile);
	fclose(OgrFile);
}
void Ogrenci_Numarasine_Gore_Ders_Listele(){
	
	FILE *OgrDers,*dersFile,*OgrFile;
	dersFile = fopen("Acilan_Dersler.txt","r+");
	OgrDers = fopen("OgrenciDersKayit.txt","r+");
	OgrFile = fopen("Ogrenciler.txt","r+");
	Dersler p;
	DersKayit d;
	Ogrenciler o;
	int OgrNum,kontrol=0;
	printf("Ogrenci numarasini girin: ");
	scanf("%d",&OgrNum);
	
	while(kontrol==0 && !feof(OgrFile)){
		
		fscanf(OgrFile,"%d  %s  %s  %d  %d\n",&o.OgrNo,o.ad,o.soyad,&o.toplamDers,&o.kredi);	
		if(o.OgrNo==OgrNum) kontrol++;
			
	}
	if(kontrol==0){
		printf("boyle bir ogrenci bulunamadi. ");
		fclose(OgrDers);
		fclose(dersFile);
		fclose(OgrFile);
		return;
	}
	printf("%s %s isimli ogrencinin aldigi dersler: ",o.ad,o.soyad);
	while(!feof(OgrDers)){
		
		fscanf(OgrDers,"%d %d %d %d   %d %d %d\n",&d.ID,&d.ogrNum,&d.dersKodu,&d.durum,&d.tarih.gun,&d.tarih.ay,&d.tarih.yil);
		if(OgrNum==d.ogrNum && d.durum==1){
			rewind(dersFile);
			while(!feof(dersFile)){
				
				fscanf(dersFile,"%d  %s  %d  %d  %d\n",&p.DersKodu,p.ad,&p.toplamKredi,&p.kontenjan,&p.OgrID);
				if(p.DersKodu==d.dersKodu){
					printf("\n%d %s",p.DersKodu,p.ad);	
				}
			}
		}
	}
	fclose(dersFile);
	fclose(OgrFile);
	fclose(OgrDers);	
}
void Sinif_Listesi_Goster(){
	
	FILE *dersSinif,*OgrDers,*OgrFile;
	int kontrol=0,dersKodu,n=0,i,j,k;
	Dersler *p;
	p=Ogretim_Uyesine_Gore_Ders_Listele(&n);
	if(p[0].DersKodu==-1) return;
	DersKayit d;
	Ogrenciler *o,x;
	printf("\nHangi dersin listesini goruntulemek istiyorsunuz: ");
	
	scanf("%d",&dersKodu);
	for(i=0;i<n;i++){
		if(p[i].DersKodu==dersKodu){
		kontrol++;
		j=i;
		}
	}
	if(kontrol==0) {
		
		printf("Yanlis Deger Girdiniz. ");
		return;	
	}
	
	char dersAdi[30];
	dersAdi[0]='\0';
	itoa(dersKodu,dersAdi,10);
	strcat(dersAdi,"_SINIFLISTESI.txt");
	OgrDers = fopen("OgrenciDersKayit.txt","r+");
	OgrFile = fopen("Ogrenciler.txt","r+");
	k=0;
	o = (Ogrenciler*)malloc(sizeof(Ogrenciler));
	while(!feof(OgrDers)){
		
		fscanf(OgrDers,"%d %d %d %d   %d %d %d\n",&d.ID,&d.ogrNum,&d.dersKodu,&d.durum,&d.tarih.gun,&d.tarih.ay,&d.tarih.yil);
		if(p[j].DersKodu==d.dersKodu && d.durum==1){
			rewind(OgrFile);
			while(!feof(OgrFile)){
				fscanf(OgrFile,"%d  %s  %s  %d  %d\n",&x.OgrNo,x.ad,x.soyad,&x.toplamDers,&x.kredi);
				if(x.OgrNo == d.ogrNum){
				
					o = (Ogrenciler*)realloc(o,sizeof(Ogrenciler)*(k+1));
					o[k].OgrNo=x.OgrNo;
					strcpy(o[k].ad,x.ad);
					strcpy(o[k].soyad,x.soyad);
					o[k].toplamDers=x.toplamDers;
					o[k].kredi=x.kredi;
					k++;
				}
			}
		}
	}
	n=k;
	dersSinif = fopen(dersAdi,"w+");
	for(i=0;i<n;i++){
		fprintf(dersSinif,"%d %s %s\n",o[i].OgrNo,o[i].ad,o[i].soyad);
		printf("\n%d %s %s",o[i].OgrNo,o[i].ad,o[i].soyad);
	}
	fclose(dersSinif);
	fclose(OgrFile);
	fclose(OgrDers);
	free(p);
	free(o);
}
int main(){
	
	int islem,islem2,OgrID,dersKodu,toplamOgr,kredi,kontrol=0;
	printf("\n\n************HOSGELDINIZ************");
	while(kontrol==0){
	printf("\n\n *************MENU***************");
	printf("\n 1 --> Ogrenci icin ekle/sil/guncelle islemleri"
	"\n 2 --> Ogretim uyeleri icin ekle/sil/guncelle islemleri"
	"\n 3 --> Dersler icin ekle/sil/guncelle islemleri"
	"\n 4 --> Ogrenci Ders kayit icin ekle/guncelle islemleri"
	"\n 5 --> Ogretim uyesine gore ders listeleme"
	"\n 6 --> Ders koduna gore ogrenci listele"
	"\n 7 --> Ogrenci numarasina gore ders listele"
	"\n 8 --> Id'si verilen ogretim uyesinin secilen dersini alan ogrencileri listele"
	"\n 0 --> Programi kapatmak icin baska bir tusa basin.");
	printf("\n\nYapmak istediginiz islemin numarasini girin: ");
	scanf("%d",&islem);
	system("CLS");
		switch(islem){
			
			case 1:
				printf("\n --> 1 Ogrenci Ekle"
				"\n --> 2 Ogrenci Sil"
				"\n --> 3 Ogrenci Guncelle");
				printf("\n\nislem secin: ");
				scanf("%d",&islem2);
				system("CLS");
				switch(islem2){
					case 1:
						OgrenciEkle();
						break;
					case 2:
						printf("\n\nsilmek istediginiz ogrencinin numarasini girin: ");
						scanf("%d",&OgrID);
						if(OgrID==-1){
							printf("yanlis deger girdiniz.");
							break;	
						} 
						else{
							OgrenciSil(OgrID);
							break;	
						}
					case 3:
						OgrenciGuncelle();
						break;
				}
				break;
			case 2:
				printf("\n --> 1 Ogretim uyesi ekle"
				"\n --> 2 Ogretim uyesi sil"
				"\n --> 3 Ogretim uyesi guncelle");
				printf("\n\nislem secin: ");
				scanf("%d",&islem2);
				system("CLS");
				switch(islem2){
					case 1:
						OgretimUyesiEkle();
						break;
					case 2:
						printf("\n\nSilmek istediginiz ogretim uyesinin ID'sini girin: ");
						scanf("%d",&OgrID);
						OgretimUyesiSil(OgrID);
						break;
					
					case 3:
						OgretimUyesiGuncelle();
						break;
				}
				break;
			case 3:
				printf("\n --> 1 Ders ekle"
				"\n --> 2 Ders sil"
				"\n --> 3 Ders guncelle");
				printf("\n\nislem secin: ");
				scanf("%d",&islem2);
				system("CLS");
				switch(islem2){
					
					case 1:
						DersEkle();
						break;	
					case 2:
						printf("\nislem yapmak istediginiz dersin kodunu girin: ");
						scanf("%d",&dersKodu);
						DersSil(dersKodu);				
						break;
					case 3:
						printf("Ogrencilerin maksimum alabilecegi kredi degerini girin: ");
						scanf("%d",&kredi);
						DersGuncelle(kredi);
						break;
				}
				break;
			case 4:
				printf("\n --> 1 Ders kayit ekle"
				"\n --> 2 Ders guncelle");
				printf("\n\nislem secin: ");
				scanf("%d",&islem2);
				system("CLS");
				switch(islem2){
					
					case 1:
						printf("Ogrencilerin maksimum alabilecegi kredi degerini girin: ");
						scanf("%d",&kredi);
						printf("Ogrencilerin toplam alabilecegi ders sayisini girin: ");
						scanf("%d",&toplamOgr);
						OgrenciDersKayit(kredi,toplamOgr);
						break;	
					case 2:
						printf("\n\nislem yapmak istediginiz ogrencinin numarasini girin: ");
						scanf("%d",&OgrID);
						printf("\nislem yapmak istediginiz dersin kodunu girin: ");
						scanf("%d",&dersKodu);
						
						OgrenciDersKayitGuncelle(OgrID,dersKodu);				
						break;
				}
				break;
			case 5:
				system("CLS");
				Ogretim_Uyesine_Gore_Ders_Listele(&islem);
				break;
			case 6:
				system("CLS");
				Ders_Koduna_Gore_Ogrenci_Listele();
				break;
			case 7:
				system("CLS");
				Ogrenci_Numarasine_Gore_Ders_Listele();
				break;
			case 8:
				system("CLS");
				Sinif_Listesi_Goster();
				break;
			default:
				kontrol++;
				break;
		}
	}
	return 0;
}

