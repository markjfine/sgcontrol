/*
 * Smart Gnome Control - database functions file
 * Copyright (C) 2004, 2005, 2025 by Mark J. Fine <mark.fine@fineware-swl.com>
 *
 * This program provides a graphical user interface to multiple
 * communications receivers using the Hamlib library.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the
 *      Free Software Foundation, Inc.
 *      59 Temple Place - Suite 330
 *      Boston, MA  02111-1307
 *      USA
 *
 */

#define _XOPEN_SOURCE
#define _DEFAULT_SOURCE

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#define nullptr ((void*)0)

#include <gtk/gtk.h>
#include <sys/stat.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#include <curl/curl.h>
#include <stdbool.h>
#include <errno.h>

#include "extra.h"
#include "database.h"
#include "logfile.h"

GSList*			readLines;
GSList*			readLines2;
GSList*			XMTRCountries;
GSList*			XMTRCodes;
GSList*			XMTRSites;
GSList*			regionList = NULL;
GSList*			statsList = NULL;
GSList*			typesList = NULL;
gchar*			inPage = "http://www.eibispace.de/";
gint			currentFilter = 0;
gchar*			currentStation = "";
gchar*			currentStationLang = "";
gchar*			currentStationTarget = "";
double			currentFrequency = 0.0;
gboolean		freqFilter = true;
gboolean		statFilter = true;

extern GtkWidget*	app1;
extern GtkWidget*	tun1;
extern GSettings*	sgc_settings;
extern gchar*		basepath;
extern gchar*		basedbpath;
extern gchar*		db_name;
extern gchar*		db_name2;
extern gint		db_filt;
extern gchar*		db_language_filt;
extern gchar*		db_country_filt;
extern gchar*		db_region_filt;
extern gchar*		db_target_filt;
extern gchar*		db_ext_name;
extern time_t		db_date;
extern gint		currentDow;
extern double		curTime;
extern int		num_freqs;
extern int		num_xmtrs;
extern gboolean		doIncludeBCB;

// FIXME - Load these lookup tables from README.TXT instead of hard-coding

gchar*	daysArray[7] = {"Mo","Tu","We","Th","Fr","Sa","Su"};

struct ITULookup_t {
  gchar*	Code;
  gchar*	Country;
};

gint	ITUCount = 354;

struct ITULookup_t	ITULookup[354] = {
  {"ABW","Aruba"},
  {"Af","Africa"},
  {"AFG","Afghanistan"},
  {"AFS","South Africa"},
  {"AGL","Angola"},
  {"AIA","Anguilla"},
  {"ALB","Albania"},
  {"ALG","Algeria"},
  {"ALS","Alaska"},
  {"Am","Americas"},
  {"AMS","St. Paul & Amsterdam Is."},
  {"AND","Andorra"},
  {"AOE","Western Sahara"},
  {"ARG","Argentina"},
  {"ARM","Armenia"},
  {"ARS","Saudi Arabia"},
  {"As","Asia"},
  {"ASC","Ascension Is."},
  {"ATA","Antarctica"},
  {"ATG","Antigua & Barbuda"},
  {"ATN","Netherlands Leeward Antilles"},
  {"AUS","Australia"},
  {"AUT","Austria"},
  {"AZE","Azerbaijan"},
  {"AZR","Azores"},
  {"B","Brasil"},
  {"BAH","Bahamas"},
  {"BDI","Burundi"},
  {"BEL","Belgium"},
  {"BEN","Benin"},
  {"BER","Bermuda"},
  {"BES","Bonaire, St. Eustatius, Saba"},
  {"BFA","Burkina Faso"},
  {"BGD","Bangla Desh"},
  {"BHR","Bahrain"},
  {"BIH","Bosnia-Herzegovina"},
  {"BIO","Chagos Is."},
  {"BLM","Saint-Barthelemy"},
  {"BLR","Belarus"},
  {"BLZ","Belize"},
  {"BOL","Bolivia"},
  {"BOT","Botswana"},
  {"BRB","Barbados"},
  {"BRU","Brunei Darussalam"},
  {"BTN","Bhutan"},
  {"BUL","Bulgaria"},
  {"BVT","Bouvet"},
  {"CAB","Cabinda"},
  {"CAf","Central Africa"},
  {"CAF","Central African Rep."},
  {"CAm","Central America"},
  {"CAN","Canada"},
  {"Car","Caribbean"},
  {"CAs","Central Asia"},
  {"Cau","Caucasus"},
  {"CBG","Cambodia"},
  {"CEu","Central Europe"},
  {"CEU","Ceuta"},
  {"CFE","Central Far East"},
  {"CG7","Guantanamo Bay"},
  {"CHL","Chile"},
  {"CHN","China (People's Rep.)"},
  {"CHR","Christmas Is."},
  {"CIS","Comm. of Ind. States"},
  {"CKH","Cook Island"},
  {"CLA","Clandestine"},
  {"CLM","Colombia"},
  {"CLN","Sri Lanka"},
  {"CME","Cameroon"},
  {"CNA","Central North America"},
  {"CNR","Canary Is."},
  {"COc","Central Oceania"},
  {"COD","Dem. Rep. of Congo (Kinshasa)"},
  {"COG","Rep. of Congo (Brazzaville)"},
  {"COM","Comores"},
  {"CPT","Clipperton"},
  {"CPV","Cape Verde Is."},
  {"CRO","Crozet Archipelago"},
  {"CTI","Ivory Coast"},
  {"CTR","Costa Rica"},
  {"CUB","Cuba"},
  {"CUW","Curacao"},
  {"CVA","Vatican State"},
  {"CYM","Cayman Is."},
  {"CYP","Cyprus"},
  {"CZE","Czech Rep."},
  {"D","Germany"},
  {"DJI","Djibouti"},
  {"DMA","Dominica"},
  {"DNK","Denmark"},
  {"DOM","Dominican Rep."},
  {"E","Spain"},
  {"EAf","East Africa"},
  {"EAm","East America"},
  {"EAs","East Asia"},
  {"EEu","East Europe"},
  {"EFE","East Far East"},
  {"EGY","Egypt"},
  {"EME","East Middle East"},
  {"ENA","Eastern North America"},
  {"ENE","East-northeast"},
  {"EOc","East Oceania"},
  {"EQA","Ecuador"},
  {"ERI","Eritrea"},
  {"ESE","East-southeast"},
  {"EST","Estonia"},
  {"ETH","Ethiopia"},
  {"Eu","Europe"},
  {"EUR","Iles Europe & Bassas da India"},
  {"F","France"},
  {"FE","Far East"},
  {"FIN","Finland"},
  {"FJI","Fiji"},
  {"FLK","Falkland Is."},
  {"FRO","Faroe Islands"},
  {"FSM","Fed. States of Micronesia"},
  {"G","United Kingdom"},
  {"GAB","Gabon"},
  {"GEO","Georgia"},
  {"GHA","Ghana"},
  {"GIB","Gibraltar"},
  {"GLP","Guadeloupe"},
  {"GMB","Gambia"},
  {"GNB","Guinea-Bissau"},
  {"GNE","Eq. Guinea"},
  {"GPG","Galapagos"},
  {"GRC","Greece"},
  {"GRD","Grenada"},
  {"GRL","Greenland"},
  {"GTM","Guatemala"},
  {"GUF","French Guyana"},
  {"GUI","Guinea"},
  {"GUM","Guam"},
  {"GUY","Guyana"},
  {"HKG","Hong Kong"},
  {"HMD","Heard & McDonald Is."},
  {"HND","Honduras"},
  {"HNG","Hungary"},
  {"HOL","The Netherlands"},
  {"HRV","Croatia"},
  {"HTI","Haiti"},
  {"HWA","Hawaii"},
  {"HWL","Howland & Baker"},
  {"I","Italy"},
  {"ICO","Cocos (Keeling) Is."},
  {"IND","India"},
  {"INS","Indonesia"},
  {"IRL","Ireland"},
  {"IRN","Iran"},
  {"IRQ","Iraq"},
  {"ISL","Iceland"},
  {"ISR","Israel"},
  {"IW","Int. Waters"},
  {"IWA","Ogasawara"},
  {"J","Japan"},
  {"JAR","Jarvis Is."},
  {"JDN","Juan de Nova"},
  {"JMC","Jamaica"},
  {"JMY","Jan Mayen"},
  {"JON","Johnston Is."},
  {"JOR","Jordan"},
  {"JUF","Juan Fernandez Is."},
  {"KAL","Kaliningrad"},
  {"KAZ","Kazakhstan"},
  {"KEN","Kenya"},
  {"KER","Kerguelen"},
  {"KGZ","Kyrgyzstan"},
  {"KIR","Kiribati"},
  {"KNA","St. Kitts & Nevis"},
  {"KOR","South Korea"},
  {"KRE","North Korea"},
  {"KWT","Kuwait"},
  {"LAm","Latin America"},
  {"LAO","Laos"},
  {"LBN","Lebanon"},
  {"LBR","Liberia"},
  {"LBY","Libya"},
  {"LCA","St. Lucia"},
  {"LIE","Liechtenstein"},
  {"LSO","Lesotho"},
  {"LTU","Lithuania"},
  {"LUX","Luxembourg"},
  {"LVA","Latvia"},
  {"MAC","Macao"},
  {"MAF","St. Martin"},
  {"MAU","Mauritius"},
  {"MCO","Monaco"},
  {"MDA","Moldova"},
  {"MDG","Madagascar"},
  {"MDR","Madeira"},
  {"MDW","Midway Is."},
  {"ME","Middle East"},
  {"MEL","Melilla"},
  {"MEX","Mexico"},
  {"MHL","Marshall Is."},
  {"MKD","Macedonia (F.Y.R.)"},
  {"MLA","Malaysia"},
  {"MLD","Maldives"},
  {"MLI","Mali"},
  {"MLT","Malta"},
  {"MNE","Montenegro"},
  {"MNG","Mongolia"},
  {"MOZ","Mozambique"},
  {"MRA","Northern Mariana Is."},
  {"MRC","Morocco"},
  {"MRN","Marion & Prince Edward Is."},
  {"MRT","Martinique"},
  {"MSR","Montserrat"},
  {"MTN","Mauritania"},
  {"MWI","Malawi"},
  {"MYA","Myanmar"},
  {"MYT","Mayotte"},
  {"NAf","North Africa"},
  {"NAm","North America"},
  {"NAO","North Atlantic Oc."},
  {"NAs","North Asia"},
  {"NCG","Nicaragua"},
  {"NCL","New Caledonia"},
  {"NEu","North Europe"},
  {"NFE","North Far East"},
  {"NFK","Norfolk Is."},
  {"NGR","Niger"},
  {"NIG","Nigeria"},
  {"NIn","North Indian Sub."},
  {"NIU","Niue"},
  {"NMB","Namibia"},
  {"NME","North Middle East"},
  {"NNE","North-northeast"},
  {"NNW","North-northwest"},
  {"NOc","North Oceania"},
  {"NOR","Norway"},
  {"NPL","Nepal"},
  {"NRU","Nauru"},
  {"NZL","New Zealand"},
  {"Oc","Oceania"},
  {"OCE","French Polynesia"},
  {"OMA","Oman"},
  {"PAK","Pakistan"},
  {"PAQ","Easter Is."},
  {"PHL","Philippines"},
  {"PHX","Phoenix Is."},
  {"PLM","Palmyra Is."},
  {"PLW","Palau"},
  {"PNG","Papua New Guinea"},
  {"PNR","Panama"},
  {"POL","Poland"},
  {"POR","Portugal"},
  {"PRG","Paraguay"},
  {"PRU","Peru"},
  {"PRV","Okino-Tori-Shima"},
  {"PSE","Palestine"},
  {"PTC","Pitcairn"},
  {"PTR","Puerto Rico"},
  {"QAT","Qatar"},
  {"REU","La Réunion"},
  {"ROD","Rodrigues"},
  {"ROU","Romania"},
  {"RRW","Rwanda"},
  {"RUS","Russian Fed."},
  {"S","Sweden"},
  {"SAf","South Africa"},
  {"SAm","South America"},
  {"SAO","South Atlantic Oc."},
  {"SAP","San Andres & Providencia"},
  {"SAs","South Asia"},
  {"SDN","Sudan"},
  {"SEA","South East Asia"},
  {"SEE","South East Europe"},
  {"SEN","Senegal"},
  {"SEu","South Europe"},
  {"SEY","Seychelles"},
  {"SFE","South Far East"},
  {"SGA","South Georgia Is."},
  {"SHN","Saint Helena"},
  {"Sib","Siberia"},
  {"SIn","South Indian Sub."},
  {"SLM","Solomon Is."},
  {"SLV","El Salvador"},
  {"SMA","Am. Samoa"},
  {"SME","South Middle East"},
  {"SMO","Samoa"},
  {"SMR","San Marino"},
  {"SNG","Singapore"},
  {"SOc","South Oceania"},
  {"SOK","South Orkney Is."},
  {"SOM","Somalia"},
  {"SPM","St. Pierre et Miquelon"},
  {"SRB","Serbia"},
  {"SRL","Sierra Leone"},
  {"SSD","South Sudan"},
  {"SSE","South-southeast"},
  {"SSI","South Sandwich Is."},
  {"SSW","South-southwest"},
  {"STP","Sao Tome & Principe"},
  {"SUI","Switzerland"},
  {"SUR","Suriname"},
  {"SVB","Svalbard"},
  {"SVK","Slovakia"},
  {"SVN","Slovenia"},
  {"SWZ","Swaziland"},
  {"SXM","St. Maarten"},
  {"SYR","Syria"},
  {"Tas","Tasmania"},
  {"TCA","Turks & Caicos Is."},
  {"TCD","Tchad"},
  {"TGO","Togo"},
  {"THA","Thailand"},
  {"Tib","Tibet"},
  {"TJK","Tajikistan"},
  {"TKL","Tokelau"},
  {"TKM","Turkmenistan"},
  {"TLS","Timor-Leste"},
  {"TON","Tonga"},
  {"TRC","Tristan da Cunha"},
  {"TRD","Trinidad & Tobago"},
  {"TUN","Tunisia"},
  {"TUR","Turkey"},
  {"TUV","Tuvalu"},
  {"TWN","Taiwan"},
  {"TZA","Tanzania"},
  {"UAE","United Arab Emirates"},
  {"UGA","Uganda"},
  {"UKR","Ukraine"},
  {"UN","United Nations"},
  {"URG","Uruguay"},
  {"USA","United States of America"},
  {"UZB","Uzbekistan"},
  {"VCT","St. Vincent & the Grenadines"},
  {"VEN","Venezuela"},
  {"VIR","Am. Virgin Is."},
  {"VRG","Br. Virgin Is."},
  {"VTN","Vietnam"},
  {"VUT","Vanuatu"},
  {"WAf","West Africa"},
  {"WAK","Wake Is."},
  {"WAL","Wallis & Futuna"},
  {"WAm","West America"},
  {"WAs","West Asia"},
  {"WEu","West Europe"},
  {"WFE","West Far East"},
  {"WIO","Western Indian Oc."},
  {"WME","West Middle East"},
  {"WNA","Western North America"},
  {"WNW","West-northwest"},
  {"WOc","West Oceania"},
  {"WSW","West-southwest"},
  {"XBY","Abyei area"},
  {"XGZ","Gaza strip"},
  {"XSP","Spratly Is."},
  {"XUU","Unidentified"},
  {"XWB","West Bank"},
  {"YEM","Yemen"},
  {"ZMB","Zambia"},
  {"ZWE","Zimbabwe"}};

struct LangLookup_t {
  gchar*	Code;
  gchar*	Language;
};

gint	langCount = 571;

struct LangLookup_t	langLookup[571] = {
  {"-CW","Morse Station"},
  {"-EC","Empty Carrier"},
  {"-HF","HFDL Squitter"},
  {"-MX","Music"},
  {"-TS","Time Signal Station"},
  {"-TY","Teletype Station"},
  {"A","Arabic"},
  {"A,E","Arabic,English"},
  {"A,F","Arabic,French"},
  {"AB","Abkhaz"},
  {"AC","Aceh"},
  {"ACH","Achang"},
  {"AD","Adygea"},
  {"ADI","Adi"},
  {"AF","Afrikaans"},
  {"AFA","Afar"},
  {"AFG","Pashto/Dari"},
  {"AH","Amharic"},
  {"AJ","Adja"},
  {"AK","Akha"},
  {"AKL","Aklanon"},
  {"AL","Albanian"},
  {"ALG","Algerian"},
  {"AM","Amoy"},
  {"AMD","Tibetan Amdo"},
  {"Ang","Angelus programme"},
  {"AR","Armenian"},
  {"ARO","Aromanian"},
  {"ARU","Languages of Arunachal, India"},
  {"ASS","Assamese"},
  {"ASY","Assyrian"},
  {"ATS","Atsi"},
  {"Aud","Papal Audience"},
  {"AV","Avar"},
  {"AW","Awadhi"},
  {"AY","Aymara"},
  {"AZ","Azeri"},
  {"BAD","Badaga"},
  {"BAG","Bagri"},
  {"BAI","Bai"},
  {"BAJ","Bajau"},
  {"BAL","Balinese"},
  {"BAN","Banjar"},
  {"BAO","Baoulé"},
  {"BAR","Bari"},
  {"BAS","Bashkir"},
  {"BAY","Bayash"},
  {"BB","Braj Bhasa"},
  {"BC","Baluchi"},
  {"BE","Bengali"},
  {"BED","Bedawiyet"},
  {"BEM","Bemba"},
  {"BET","Bété"},
  {"BGL","Bagheli"},
  {"BH","Bhili"},
  {"BHN","Bahnar"},
  {"BHT","Bhatri"},
  {"BI","Bilen"},
  {"BID","Bidayuh languages"},
  {"BIS","Bisaya"},
  {"BJ","Bhojpuri"},
  {"BK","Balkarian"},
  {"BLK","Balkan Romani"},
  {"BLT","Balti"},
  {"BM","Bambara"},
  {"BNA","Borana Oromo"},
  {"BNG","Bangala"},
  {"BNI","Baniua"},
  {"BNJ","Banjari"},
  {"BNT","Bantawa"},
  {"BON","Bondo"},
  {"BOR","Boro"},
  {"BOS","Bosnian"},
  {"BR","Burmese"},
  {"BRA","Brahui"},
  {"BRB","Bariba"},
  {"BRU","Bru"},
  {"BSL","Bislama"},
  {"BT","Black Tai"},
  {"BTK","Batak-Toba"},
  {"BU","Bulgarian"},
  {"BUG","Bugis"},
  {"BUK","Bukharian"},
  {"BUN","Bundeli"},
  {"BUR","Buryat"},
  {"BY","Byelorussian"},
  {"C","Chinese"},
  {"CA","Cantonese"},
  {"CC","Chaochow"},
  {"CD","Chowdary"},
  {"CEB","Cebuano"},
  {"CH","Chin"},
  {"C-A","Chin-Asho"},
  {"C-D","Chin-Daai"},
  {"C-F","Chin-Falam"},
  {"C-H","Chin-Haka"},
  {"CHA","Cha'palaa"},
  {"CHE","Chechen"},
  {"CHG","Chhattisgarhi"},
  {"CHI","Chitrali"},
  {"C-K","Chin-Khumi"},
  {"C-M","Chin-Mro"},
  {"C-O","Chin-Thado"},
  {"CHR","Chrau"},
  {"CHU","Chuwabu"},
  {"C-T","Chin-Tidim"},
  {"C-Z","Chin-Zomin"},
  {"CKM","Chakma"},
  {"CKW","Chokwe"},
  {"COF","Cofan"},
  {"COK","Cook Is. Maori"},
  {"CR","Creole"},
  {"CRU","Chru"},
  {"CT","Catalan"},
  {"CV","Chuvash"},
  {"CVC","Chavacano"},
  {"CW","Chewa"},
  {"CZ","Czech"},
  {"D","German"},
  {"D,E","German,English"},
  {"D-P","Lower German"},
  {"DA","Danish"},
  {"DAH","Dahayia"},
  {"DAO","Dao"},
  {"DAR","Dargwa"},
  {"DD","Dhodiya"},
  {"DEC","Deccan"},
  {"DEG","Degar"},
  {"DEN","Dendi"},
  {"DEO","Deori"},
  {"DES","Desiya"},
  {"DH","Dhivehi"},
  {"DI","Dinka"},
  {"DIM","Dimasa"},
  {"DIT","Ditamari"},
  {"DO","Dogri-Kangri"},
  {"DR","Dari"},
  {"DU","Dusun"},
  {"DUN","Dungan"},
  {"DY","Dyula"},
  {"DZ","Dzongkha"},
  {"E","English"},
  {"E,D","English,German"},
  {"E,F","English,French"},
  {"E,J","English,Japanese"},
  {"E,M","English,Mandarin"},
  {"E,R","English,Russian"},
  {"E,S","English,Spanish"},
  {"E,T","English,Thai"},
  {"E,Z","English,Zulu"},
  {"EC","Eastern Cham"},
  {"EGY","Egyptian Arabic"},
  {"EO","Esperanto"},
  {"ES","Estonian"},
  {"EWE","Ewe"},
  {"F","French"},
  {"F,E","French,English"},
  {"F,M","French,Mandarin"},
  {"FA","Faroese"},
  {"FI","Finnish"},
  {"FJ","Fijian"},
  {"FON","Fon"},
  {"FP","Filipino"},
  {"FS","Farsi"},
  {"FT","Fiote"},
  {"FU","Fulani"},
  {"FUJ","FutaJalon"},
  {"FUR","Fur"},
  {"GA","Garhwali"},
  {"GAG","Gagauz"},
  {"GAR","Garo"},
  {"GD","Greenlandic Inuktikut"},
  {"GE","Georgian"},
  {"GI","Gilaki"},
  {"GJ","Gujari"},
  {"GL","Galicic"},
  {"GM","Gamit"},
  {"GNG","Gurung"},
  {"GO","Gorontalo"},
  {"GON","Gondi"},
  {"GR","Greek"},
  {"GU","Gujarati"},
  {"GUA","Guaraní"},
  {"GUN","Gungbe"},
  {"GUR","Gurage"},
  {"GZ","Ge'ez"},
  {"HA","Haussa"},
  {"HAD","Hadiya"},
  {"HAR","Haryanvi"},
  {"HAS","Hassinya"},
  {"HB","Hebrew"},
  {"HD","Hindko"},
  {"HI","Hindi"},
  {"HIM","Himachali"},
  {"HK","Hakka"},
  {"HM","Hmong"},
  {"HMA","Hmar"},
  {"HMB","Hmong-Blue"},
  {"HMQ","Hmong"},
  {"HMW","Hmong-White"},
  {"HN","Hani"},
  {"HO","Ho"},
  {"HR","Croatian"},
  {"HRE","Hre"},
  {"HU","Hungarian"},
  {"HUI","Hui"},
  {"HZ","Hazaragi"},
  {"I","Italian"},
  {"IB","Iban"},
  {"IBN","Ibanag"},
  {"IF","Ifè"},
  {"IG","Igbo"},
  {"ILC","Ilocano"},
  {"ILG","Ilonggo"},
  {"IN","Indonesian"},
  {"INU","Inuktikut"},
  {"IRQ","Iraqi Arabic"},
  {"IS","Icelandic"},
  {"ISA","Isan"},
  {"ITA","Itawis"},
  {"J","Japanese"},
  {"J,E","Japanese,English"},
  {"JAI","Jaintia"},
  {"JEH","Jeh"},
  {"JG","Jingpho"},
  {"JOR","Jordanian Arabic"},
  {"JR","Jarai"},
  {"JU","Juba Arabic"},
  {"JV","Javanese"},
  {"K","Korean"},
  {"K,E","Korean,English"},
  {"KA","Karen"},
  {"K-G","Karen-Geba"},
  {"K-K","Karen-Geko"},
  {"K-M","Karen-Manumanaw"},
  {"K-P","Karen-Pao"},
  {"K-S","Karen-Sgaw"},
  {"K-W","Karen-Pwo"},
  {"KAD","Kadazan"},
  {"KAL","Kalderash Romani"},
  {"KAB","Kabardian"},
  {"KAM","Kambaata"},
  {"KAN","Kannada"},
  {"KAO","Kaonde"},
  {"KAR","Karelian"},
  {"KAT","Katu"},
  {"KAU","Kau Bru"},
  {"KAY","Kayan"},
  {"KB","Kabyle"},
  {"KBO","Kok Borok"},
  {"KC","Kachin"},
  {"KG","Kyrgyz"},
  {"KGU","Kalanguya"},
  {"KH","Khmer"},
  {"KHA","Kham"},
  {"KHM","Khmu"},
  {"KHR","Kharia"},
  {"KHS","Khasi"},
  {"KHT","Khota"},
  {"KIM","Kimwani"},
  {"KIN","Kinnauri"},
  {"KiR","KiRundi"},
  {"KK","KiKongo"},
  {"KKA","Kankana-ey"},
  {"KKN","Kukna"},
  {"KKU","Korku"},
  {"KMB","Kimbundu"},
  {"KMY","Kumyk"},
  {"KND","Khandesi"},
  {"KNG","Kangri"},
  {"KNK","KinyaRwanda"},
  {"KNU","Kanuri"},
  {"KNY","Konyak Naga"},
  {"KOH","Koho"},
  {"KOK","Kokang Shan"},
  {"KOM","Komering"},
  {"KON","Konkani"},
  {"KOR","Korambar"},
  {"KOT","Kotokoli"},
  {"KOY","Koya"},
  {"KPK","Karakalpak"},
  {"KRB","Karbi"},
  {"KRI","Krio"},
  {"KRW","KinyaRwanda"},
  {"KRY","Karay-a"},
  {"KS","Kashmiri"},
  {"KT","Kituba"},
  {"KTW","Kotwali"},
  {"KU","Kurdish"},
  {"KuA","Kurdish & Arabic"},
  {"KuF","Kurdish & Farsi"},
  {"KUI","Kui"},
  {"KUL","Kulina"},
  {"KUM","Kumaoni"},
  {"KUN","Kunama"},
  {"KUP","Kupia"},
  {"KUR","Kurukh"},
  {"KUs","Sorani Kurdish"},
  {"KUT","Kutchi"},
  {"KUV","Kuvi"},
  {"KVI","Kulluvi"},
  {"KWA","Kwanyama"},
  {"KYH","Kayah"},
  {"KZ","Kazakh"},
  {"L","Latin"},
  {"LA","Ladino"},
  {"LAD","Ladakhi"},
  {"LAH","Lahu"},
  {"LAK","Lak"},
  {"LAM","Lampung"},
  {"LAO","Lao"},
  {"LB","Lun Bawang"},
  {"LBN","Lebanon Arabic"},
  {"LBO","Limboo"},
  {"LEP","Lepcha"},
  {"LEZ","Lezgi"},
  {"LIM","Limba"},
  {"LIN","Lingala"},
  {"LIS","Lisu"},
  {"LND","Lunda"},
  {"LNG","Lungeli Magar"},
  {"LO","Lomwe"},
  {"LOK","Lokpa"},
  {"LOZ","Lozi"},
  {"LT","Lithuanian"},
  {"LTO","Oriental Liturgy"},
  {"LU","Lunda"},
  {"LUB","Luba"},
  {"LUC","Luchazi"},
  {"LUG","Luganda"},
  {"LUN","Lunyaneka"},
  {"LUR","Luri"},
  {"LUV","Luvale"},
  {"LV","Latvian"},
  {"M","Mandarin"},
  {"M,E","Mandarin,English"},
  {"MA","Maltese"},
  {"MAD","Madurese"},
  {"MAG","Maghi"},
  {"MAI","Maithili"},
  {"MAK","Makonde"},
  {"MAL","Malayalam"},
  {"MAM","Maay"},
  {"MAN","Mandenkan"},
  {"MAO","Maori"},
  {"MAR","Marathi"},
  {"MAS","Maasai"},
  {"MC","Macedonian"},
  {"MCH","Mavchi"},
  {"MEI","Meithei"},
  {"MEN","Mende"},
  {"MEW","Mewari"},
  {"MGA","Magar"},
  {"MIE","Mien"},
  {"MIS","Mising"},
  {"MKB","Minangkabau"},
  {"MKS","Makassar"},
  {"MKU","Makua"},
  {"ML","Malay"},
  {"MLK","Malinke"},
  {"MLT","Malto"},
  {"MNA","Mina"},
  {"MNB","Manobo"},
  {"MNE","Montenegrin"},
  {"MNO","Mnong"},
  {"MO","Mongolian"},
  {"MON","Mon"},
  {"MOO","Moore"},
  {"MOR","Moro"},
  {"MR","Maronite"},
  {"MRC","Moroccan"},
  {"MRI","Mari"},
  {"MRU","Maru"},
  {"MSY","Malagasy"},
  {"MUN","Mundari"},
  {"MUO","Muong"},
  {"MUR","Murut"},
  {"MV","Malvi"},
  {"MW","Marwari"},
  {"MX","Macuxi"},
  {"MY","Maya"},
  {"MZ","Mizo"},
  {"NAG","Naga"},
  {"NAP","Naga Pidgin"},
  {"NDA","Ndau"},
  {"NDE","Ndebele"},
  {"NE","Nepali"},
  {"NG","Nagpuri"},
  {"NGA","Ngangela"},
  {"NIC","Nicobari"},
  {"NIG","Nigerian Pidgin"},
  {"NIS","Nishi"},
  {"NIU","Niuean"},
  {"NJ","Ngaju"},
  {"NL","Dutch"},
  {"NLA","Nga La"},
  {"NO","Norwegian"},
  {"NOC","Nocte"},
  {"NP","Nupe"},
  {"NTK","Natakani"},
  {"NU","Nuer"},
  {"NUN","Nung"},
  {"NW","Newar"},
  {"NY","Nyanja"},
  {"OG","Ogan"},
  {"OH","Otjiherero"},
  {"OO","Oromo"},
  {"OR","Odia"},
  {"OS","Ossetic"},
  {"OW","Oshiwambo"},
  {"P","Portuguese"},
  {"P,E","Portuguese,English"},
  {"PAL","Palaung - Pale"},
  {"PAS","Pasemah"},
  {"PED","Pedi"},
  {"PJ","Punjabi"},
  {"PO","Polish"},
  {"POR","Po"},
  {"POT","Pothwari"},
  {"PS","Pashto"},
  {"PU","Pulaar"},
  {"Q","Quechua"},
  {"Q,S","Quechua,Spanish"},
  {"QQ","Qashqai"},
  {"R","Russian"},
  {"RAD","Rade"},
  {"REN","Rengao"},
  {"RGM","Rengma Naga"},
  {"RO","Romanian"},
  {"ROG","Roglai"},
  {"ROH","Rohingya"},
  {"RON","Rongmei Naga"},
  {"Ros","Rosary session"},
  {"RU","Rusyn"},
  {"RWG","Rawang"},
  {"S","Spanish"},
  {"S,E","Spanish,English"},
  {"S,Q","Spanish,Quechua"},
  {"SAH","Saho"},
  {"SAN","Sango"},
  {"SAR","Sara"},
  {"SAS","Sasak"},
  {"SC","Serbocroat"},
  {"SCA","Scandinavian languages"},
  {"SD","Sindhi"},
  {"SED","Sedang"},
  {"SEF","Sefardi"},
  {"SEN","Sena"},
  {"SFO","Senoufo"},
  {"SGA","Shangaan"},
  {"SGM","Sara Gambai"},
  {"SGO","Songo"},
  {"SGT","Sangtam"},
  {"SHA","Shan"},
  {"SHk","Shan-Khamti"},
  {"SHC","Sharchogpa"},
  {"SHE","Sheena"},
  {"SHK","Shiluk"},
  {"SHO","Shona"},
  {"SHP","Sherpa"},
  {"SHU","Shuwa Arabic"},
  {"SI","Sinhalese"},
  {"SID","Sidamo"},
  {"SIK","Sikkimese"},
  {"SIR","Siraiki"},
  {"SK","Slovak"},
  {"SLM","Pijin"},
  {"SLT","Silte"},
  {"SM","Samoan"},
  {"SMP","Sambalpuri"},
  {"SNK","Sanskrit"},
  {"SNT","Santhali"},
  {"SO","Somali"},
  {"SON","Songhai"},
  {"SOT","SeSotho"},
  {"SR","Serbian"},
  {"SRA","Soura"},
  {"STI","Stieng"},
  {"SUA","Shuar"},
  {"SUD","Sudanese Arabic"},
  {"SUM","Sumi Naga"},
  {"SUN","Sunda"},
  {"SUR","Surgujia"},
  {"SUS","Sudan Service of IBRA"},
  {"SV","Slovenian"},
  {"SWA","Swahili"},
  {"SWE","Swedish"},
  {"SWZ","SiSwati"},
  {"T","Thai"},
  {"T,R","Thai,Russian"},
  {"TAG","Tagalog"},
  {"TAH","Tachelhit"},
  {"TAL","Talysh"},
  {"TAM","Tamil"},
  {"TAU","Tausug"},
  {"TB","Tibetan"},
  {"TBL","Tboli"},
  {"TBS","Tabasaran"},
  {"TEL","Telugu"},
  {"TEM","Temme"},
  {"TFT","Tarifit"},
  {"TGB","Tagabawa"},
  {"TGK","Tangkhul"},
  {"TGR","Tigre"},
  {"TGS","Tangsa"},
  {"THA","Tharu Buksa"},
  {"TIG","Tigrinya"},
  {"TJ","Tajik"},
  {"TK","Turkmen"},
  {"TKL","Tagakaulo"},
  {"TL","Tai-Lu"},
  {"TM","Tamazight"},
  {"TMG","Tamang"},
  {"TMJ","Tamajeq"},
  {"TN","Tai-Nua"},
  {"TNG","Tonga"},
  {"TO","Tongan"},
  {"TOK","Tokelau"},
  {"TOR","Torajanese"},
  {"TP","Tok Pisin"},
  {"TS","Tswana"},
  {"TSA","Tsangla"},
  {"TSH","Tshwa"},
  {"TT","Tatar"},
  {"TTB","Tatar-Bashkir"},
  {"TU","Turkish"},
  {"TUL","Tulu"},
  {"TUM","Tumbuka"},
  {"TUN","Tunisian Arabic"},
  {"TUR","Turkana"},
  {"TV","Tuva"},
  {"TW","Taiwanese"},
  {"TWI","Twi"},
  {"TWT","Tachawit"},
  {"TZ","Tamazight"},
  {"UA","Dawan"},
  {"UD","Udmurt"},
  {"UI","Uighur"},
  {"UK","Ukrainian"},
  {"UM","Umbundu"},
  {"UR","Urdu"},
  {"UZ","Uzbek"},
  {"V","Vasco"},
  {"VAD","Vadari"},
  {"VAR","Varli"},
  {"Ves","Vespers"},
  {"Vn","Vernacular"},
  {"VN","Vietnamese"},
  {"VV","Vasavi"},
  {"VX","Vlax Romani"},
  {"W","Wolof"},
  {"WA","Wa"},
  {"WAO","Waodani"},
  {"WE","Wenzhou"},
  {"WT","White Tai"},
  {"WU","Wu"},
  {"XH","Xhosa"},
  {"YAO","Yao"},
  {"YER","Yerukula"},
  {"YI","Yi"},
  {"YK","Yakutian"},
  {"YO","Yoruba"},
  {"YOL","Yolngu"},
  {"YUN","Dialects of Yunnan"},
  {"YZ","Yezidi program"},
  {"Z","Zulu"},
  {"ZA","Zarma"},
  {"ZD","Zande"},
  {"ZG","Zaghawa"},
  {"ZH","Zhuang"},
  {"ZWE","Languages of Zimbabwe"}};

struct regionLookup_t {
  gchar*	Code;
  gchar*	Region;
};

gint	regionCount = 373;

struct regionLookup_t	regionLookup[373] = {
  {"ABW","N. America"},
  {"Af","Africa"},
  {"AFG","Asia"},
  {"AFS","Africa"},
  {"AGL","Africa"},
  {"AIA","N. America"},
  {"ALB","Europe"},
  {"ALG","Africa"},
  {"ALS","N. America"},
  {"Am","N. America"},
  {"Am","S. America"},
  {"AMS","Africa"},
  {"AMS","Antarctica"},
  {"AND","Europe"},
  {"AOE","Africa"},
  {"ARG","S. America"},
  {"ARM","Europe"},
  {"ARM","Asia"},
  {"ARS","Asia"},
  {"As","Asia"},
  {"ASC","Africa"},
  {"ASC","S. America"},
  {"ATA","Antarctica"},
  {"ATG","N. America"},
  {"ATN","N. America"},
  {"AUS","Oceania"},
  {"AUT","Europe"},
  {"AZE","Asia"},
  {"AZE","Europe"},
  {"AZR","Europe"},
  {"B","S. America"},
  {"BAH","N. America"},
  {"BDI","Africa"},
  {"BEL","Europe"},
  {"BEN","Africa"},
  {"BER","N. America"},
  {"BES","N. America"},
  {"BFA","Africa"},
  {"BGD","Asia"},
  {"BHR","Asia"},
  {"BIH","Europe"},
  {"BIO","Africa"},
  {"BIO","Asia"},
  {"BLM","N. America"},
  {"BLR","Europe"},
  {"BLZ","N. America"},
  {"BOL","S. America"},
  {"BOT","Africa"},
  {"BRB","N. America"},
  {"BRU","Asia"},
  {"BTN","Asia"},
  {"BUL","Europe"},
  {"BVT","Africa"},
  {"BVT","Antarctica"},
  {"CAB","Africa"},
  {"CAf","Africa"},
  {"CAF","Africa"},
  {"CAm","N. America"},
  {"CAN","N. America"},
  {"Car","N. America"},
  {"CAs","Asia"},
  {"Cau","Europe"},
  {"CBG","Asia"},
  {"CEu","Europe"},
  {"CEU","Africa"},
  {"CFE","Asia"},
  {"CG7","N. America"},
  {"CHL","S. America"},
  {"CHN","Asia"},
  {"CHR","Asia"},
  {"CIS","Asia"},
  {"CIS","Europe"},
  {"CKH","Oceania"},
  {"CLA",""},
  {"CLM","S. America"},
  {"CLN","Asia"},
  {"CME","Africa"},
  {"CNA","N. America"},
  {"CNR","Africa"},
  {"COc","Oceania"},
  {"COD","Africa"},
  {"COG","Africa"},
  {"COM","Africa"},
  {"CPT","N. America"},
  {"CPV","Africa"},
  {"CRO","Africa"},
  {"CRO","Antarctica"},
  {"CTI","Africa"},
  {"CTR","N. America"},
  {"CUB","N. America"},
  {"CUW","N. America"},
  {"CVA","Europe"},
  {"CYM","N. America"},
  {"CYP","Asia"},
  {"CYP","Europe"},
  {"CZE","Europe"},
  {"D","Europe"},
  {"DJI","Africa"},
  {"DMA","N. America"},
  {"DNK","Europe"},
  {"DOM","N. America"},
  {"E","Europe"},
  {"EAf","Africa"},
  {"EAm","N. America"},
  {"EAm","S. America"},
  {"EAs","Asia"},
  {"EEu","Europe"},
  {"EFE","Asia"},
  {"EGY","Africa"},
  {"EME","Asia"},
  {"ENA","N. America"},
  {"EOc","Oceania"},
  {"EQA","S. America"},
  {"ERI","Africa"},
  {"EST","Europe"},
  {"ETH","Africa"},
  {"Eu","Europe"},
  {"EUR","Africa"},
  {"F","Europe"},
  {"FE","Asia"},
  {"FIN","Europe"},
  {"FJI","Oceania"},
  {"FLK","S. America"},
  {"FRO","Europe"},
  {"FSM","Oceania"},
  {"G","Europe"},
  {"GAB","Africa"},
  {"GEO","Asia"},
  {"GEO","Europe"},
  {"GHA","Africa"},
  {"GIB","Europe"},
  {"GLP","N. America"},
  {"GMB","Africa"},
  {"GNB","Africa"},
  {"GNE","Africa"},
  {"GPG","S. America"},
  {"GRC","Europe"},
  {"GRD","N. America"},
  {"GRL","N. America"},
  {"GTM","N. America"},
  {"GUF","S. America"},
  {"GUI","Africa"},
  {"GUM","Oceania"},
  {"GUY","S. America"},
  {"HKG","Asia"},
  {"HMD","Antarctica"},
  {"HND","N. America"},
  {"HNG","Europe"},
  {"HOL","Europe"},
  {"HRV","Europe"},
  {"HTI","N. America"},
  {"HWA","Oceania"},
  {"HWL","Oceania"},
  {"I","Europe"},
  {"ICO","Asia"},
  {"ICO","Oceania"},
  {"IND","Asia"},
  {"INS","Asia"},
  {"IRL","Europe"},
  {"IRN","Asia"},
  {"IRQ","Asia"},
  {"ISL","Europe"},
  {"ISR","Asia"},
  {"IW",""},
  {"IWA","Asia"},
  {"J","Asia"},
  {"JAR","Oceania"},
  {"JDN","Africa"},
  {"JMC","N. America"},
  {"JMY","Europe"},
  {"JMY","N. America"},
  {"JON","Oceania"},
  {"JOR","Asia"},
  {"JUF","S. America"},
  {"KAL","Asia"},
  {"KAL","Europe"},
  {"KAZ","Asia"},
  {"KAZ","Europe"},
  {"KEN","Africa"},
  {"KER","Africa"},
  {"KER","Antarctica"},
  {"KGZ","Asia"},
  {"KIR","Oceania"},
  {"KNA","N. America"},
  {"KOR","Asia"},
  {"KRE","Asia"},
  {"KWT","Asia"},
  {"LAm","N. America"},
  {"LAm","S. America"},
  {"LAO","Asia"},
  {"LBN","Asia"},
  {"LBR","Africa"},
  {"LBY","Africa"},
  {"LCA","N. America"},
  {"LIE","Europe"},
  {"LSO","Africa"},
  {"LTU","Europe"},
  {"LUX","Europe"},
  {"LVA","Europe"},
  {"MAC","Asia"},
  {"MAF","N. America"},
  {"MAU","Africa"},
  {"MCO","Europe"},
  {"MDA","Europe"},
  {"MDG","Africa"},
  {"MDR","Africa"},
  {"MDW","Oceania"},
  {"ME","Asia"},
  {"MEL","Africa"},
  {"MEX","N. America"},
  {"MHL","Oceania"},
  {"MKD","Europe"},
  {"MLA","Asia"},
  {"MLD","Asia"},
  {"MLI","Africa"},
  {"MLT","Europe"},
  {"MNE","Europe"},
  {"MNG","Asia"},
  {"MOZ","Africa"},
  {"MRA","Oceania"},
  {"MRC","Africa"},
  {"MRN","Africa"},
  {"MRN","Antarctica"},
  {"MRT","N. America"},
  {"MSR","N. America"},
  {"MTN","Africa"},
  {"MWI","Africa"},
  {"MYA","Asia"},
  {"MYT","Africa"},
  {"NAf","Africa"},
  {"NAm","N. America"},
  {"NAO","Europe"},
  {"NAO","N. America"},
  {"NAs","Asia"},
  {"NCG","N. America"},
  {"NCL","Oceania"},
  {"NEu","Europe"},
  {"NFE","Asia"},
  {"NFK","Oceania"},
  {"NGR","Africa"},
  {"NIG","Africa"},
  {"NIn","Asia"},
  {"NIU","Oceania"},
  {"NMB","Africa"},
  {"NME","Asia"},
  {"NOc","Oceania"},
  {"NOR","Europe"},
  {"NPL","Asia"},
  {"NRU","Oceania"},
  {"NZL","Oceania"},
  {"Oc","Oceania"},
  {"OCE","Oceania"},
  {"OMA","Asia"},
  {"PAK","Asia"},
  {"PAQ","Oceania"},
  {"PAQ","S. America"},
  {"PHL","Asia"},
  {"PHX","Asia"},
  {"PLM","Oceania"},
  {"PLW","Oceania"},
  {"PNG","Oceania"},
  {"PNR","N. America"},
  {"POL","Europe"},
  {"POR","Europe"},
  {"PRG","S. America"},
  {"PRU","S. America"},
  {"PRV","Asia"},
  {"PSE","Asia"},
  {"PTC","Oceania"},
  {"PTR","N. America"},
  {"QAT","Asia"},
  {"REU","Africa"},
  {"ROD","Africa"},
  {"ROU","Europe"},
  {"RRW","Africa"},
  {"RUS","Asia"},
  {"RUS","Europe"},
  {"S","Europe"},
  {"SAf","Africa"},
  {"SAm","S. America"},
  {"SAO","Africa"},
  {"SAO","Antarctica"},
  {"SAO","S. America"},
  {"SAP","N. America"},
  {"SAs","Asia"},
  {"SDN","Africa"},
  {"SEA","Asia"},
  {"SEE","Europe"},
  {"SEN","Africa"},
  {"SEu","Europe"},
  {"SEY","Africa"},
  {"SFE","Asia"},
  {"SGA","S. America"},
  {"SHN","Africa"},
  {"Sib","Asia"},
  {"SIn","Asia"},
  {"SLM","Oceania"},
  {"SLV","N. America"},
  {"SMA","Oceania"},
  {"SME","Asia"},
  {"SMO","Oceania"},
  {"SMR","Europe"},
  {"SNG","Asia"},
  {"SOc","Oceania"},
  {"SOK","Antarctica"},
  {"SOM","Africa"},
  {"SPM","N. America"},
  {"SRB","Europe"},
  {"SRL","Africa"},
  {"SSD","Africa"},
  {"SSI","S. America"},
  {"STP","Africa"},
  {"SUI","Europe"},
  {"SUR","S. America"},
  {"SVB","Europe"},
  {"SVB","N. America"},
  {"SVK","Europe"},
  {"SVN","Europe"},
  {"SWZ","Africa"},
  {"SXM","N. America"},
  {"SYR","Asia"},
  {"Tas","Oceania"},
  {"TCA","N. America"},
  {"TCD","Africa"},
  {"TGO","Africa"},
  {"THA","Asia"},
  {"Tib","Asia"},
  {"TJK","Asia"},
  {"TKL","Oceania"},
  {"TKM","Asia"},
  {"TLS","Asia"},
  {"TON","Oceania"},
  {"TRC","Africa"},
  {"TRD","N. America"},
  {"TUN","Africa"},
  {"TUR","Asia"},
  {"TUR","Europe"},
  {"TUV","Oceania"},
  {"TWN","Asia"},
  {"TZA","Africa"},
  {"UAE","Asia"},
  {"UGA","Africa"},
  {"UKR","Europe"},
  {"UN",""},
  {"URG","S. America"},
  {"USA","N. America"},
  {"UZB","Asia"},
  {"VCT","N. America"},
  {"VEN","S. America"},
  {"VIR","N. America"},
  {"VRG","N. America"},
  {"VTN","Asia"},
  {"VUT","Oceania"},
  {"WAf","Africa"},
  {"WAK","Oceania"},
  {"WAL","Oceania"},
  {"WAm","N. America"},
  {"WAm","S. America"},
  {"WAs","Asia"},
  {"WEu","Europe"},
  {"WFE","Asia"},
  {"WIO","Africa"},
  {"WME","Asia"},
  {"WNA","N. America"},
  {"WOc","Oceania"},
  {"XBY","Africa"},
  {"XGZ","Asia"},
  {"XSP","Asia"},
  {"XUU",""},
  {"XWB","Asia"},
  {"YEM","Asia"},
  {"ZMB","Africa"},
  {"ZWE","Africa"}};

#ifdef __MINGW64__
char*
strptime(const char* restrict inStr, const char* restrict inFmt, struct tm* tm)
{
   gchar*	timebit;
   gchar*	timebits[3] = {"","",""};
   gchar*	timefmt;
   gchar*	timefmts[3] = {"","",""};
   char		smonths[12][3] = {"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};
   gint		month = -1;
   char		moStr[3];
   gint		day = 0;
   gint		year = 1900;
   gint		i;
   gint		j;
   gchar*	inStr2;
   gchar*	inFmt2;
   
   inStr2 = strdup(inStr);
   inFmt2 = strdup(inFmt);
   
   i = 0;
   timebit = strtok(inStr2, " ");
   while (timebit != NULL) {
     timebits[i] = strdup(timebit);
     i = i + 1;
     timebit = strtok(NULL, " ");
   }
   
   i = 0;
   timefmt = strtok(inFmt2, " ");
   while (timefmt != NULL) {
     timefmts[i] = strdup(timefmt);
     i = i + 1;
     timefmt = strtok(NULL, " ");
   }
   
   for (j = 0; j < 3; j++) {
     switch (timefmts[j][1]) {
     
       case 'b' : //3-letter month
         sscanf(timebits[j],"%3c",moStr);
         for (i=0; i<3; i++)
           moStr[i] = toupper(moStr[i]);
         for (i = 0; i < 12; i++)
           if ((moStr[0] == smonths[i][0]) && (moStr[1] == smonths[i][1]) && (moStr[2] == smonths[i][2]))
             month = i;
         break;
         
       case 'd' : //2-digit day
         sscanf(timebits[j],"%d",&day);
         break;
       
       case 'Y' : //4-digit year
         sscanf(timebits[j],"%d",&year);
         break;
         
       default :
         break;
     }
   }
   
   g_free(inStr2);
   g_free(inFmt2);
   for (i = 0; i < 3; i++) {
     g_free(timebits[i]);
     g_free(timefmts[i]);
   }

   if (month == -1) {
     return NULL;
   }

   tm->tm_mon = month;
   tm->tm_mday = day;
   tm->tm_year = year - 1900;
   
   return inStr+strlen(inStr)-1;
}
#endif

void
get_database_file()
{
  GtkWidget*		file_selector;
  GtkFileFilter*	file_filter;
  gchar*		temp;
  gint			result;
  
  file_selector = gtk_file_chooser_dialog_new("Select Database File",
                                              GTK_WINDOW(app1),
                                              GTK_FILE_CHOOSER_ACTION_OPEN,
                                              "_Cancel",
                                              GTK_RESPONSE_CANCEL,
                                              "_Open",
                                              GTK_RESPONSE_ACCEPT,
                                              NULL);
  gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(file_selector), basepath);
  file_filter = gtk_file_filter_new();
  gtk_file_filter_add_pattern(file_filter, "eibi-freq-*.txt");
  gtk_file_filter_set_name(file_filter, "Eibi database files (eibi-freq-*.txt)");
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(file_selector), file_filter);
  gtk_window_set_position(GTK_WINDOW(file_selector), GTK_WIN_POS_CENTER);
  gtk_window_set_modal(GTK_WINDOW(file_selector), TRUE);
  result = gtk_dialog_run(GTK_DIALOG(file_selector));
  if (result == GTK_RESPONSE_ACCEPT) {
    temp = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_selector));
    db_name = g_strdup(temp);
#ifdef __MINGW64__
      replace_backslashes(db_name);
#endif          
    open_database();
    g_free(temp);
  }
  gtk_widget_destroy(file_selector);
}


gboolean
dayValid				(gchar*	inDay,
                                         gchar*	inType)
{
  gchar*	letsString = "FMSTWaehoru,-";
  gchar*	numsString = "1234567";
  gboolean	isValid = true;
  gchar*	dayArray = inDay;
  gchar*	inString;
  gchar*	pos;
  gint		i;
  
  if (strcmp(inType,"lets") == 0)
    inString = letsString;
  else
    inString = numsString;

  for (i = 0; i < strlen(dayArray); i++)  {
    pos = strchr(inString, dayArray[i]);
    if (pos == NULL) {
      isValid = false;
      break;
    }
  }
  return isValid;
}

guint
day2Int					(gchar*	inDay)
{
  gint		pos;
  gint		i;
  guint		result;
  
  pos = -1;
  for (i = 0; i < 7; i++) {
    if (strcmp(daysArray[i], inDay) == 0)
      pos = i;
  }
  result = 0;
  if (pos != -1)
    result = (pow(2.0,(double)pos));
  return result;
}

gchar*
trim					(gchar*	str)
{
  gchar*	start = str;
  gchar*	end = str + strlen(str);
  
  if (str == NULL) {
    return NULL;
  }
  
  if (str[0] == '\0') {
    return "";
  }
  
  if (str[0] == ' ') {
    while (isspace(*start) && (start < end))
      start++;
  }

  if (str[strlen(str)-1] == ' ') {
    if (start < end) {
      end --;        
      while (isspace(*end) && (end > start)) {
        end--;
      }
      end[1] = '\0';
    } else {
      str[0] = '\0';
    }
  }

  if (start != str)
    memmove(str, start, (end - start) + 2);

  return str;
}


gint
utf8_char_len				(unsigned char c)
{
  if (c < 0x80)
    return 1;
  if (c < 0xE0)
    return 2;
  if (c < 0xF0)
    return 3;
  return 4;
}


gchar*
substr					(const gchar* str,
                                         int start,
                                         int length)
{
  gint		str_len = strlen(str);
  gchar*	sub;
  gint		char_len;
  gint		sub_len;
  gint		char_count = 0;
  gint		byte_pos = 0;
  gint		start_byte = -1;
  gint		end_byte = -1;
  
  if ((str == NULL) || (start < 0) || (length <= 0))
    return NULL;

  while ((byte_pos < str_len) && (char_count < start)) {
    char_len = utf8_char_len((unsigned char)str[byte_pos]);
    byte_pos += char_len;
    char_count++;
  }

  start_byte = byte_pos;
  if (start_byte >= str_len)
    return NULL;

  char_count = 0;
  while ((byte_pos < str_len) && (char_count < length)) {
    char_len = utf8_char_len((unsigned char)str[byte_pos]);
    byte_pos += char_len;
    char_count++;
  }
  end_byte = byte_pos;

  sub_len = end_byte - start_byte;
  sub = (gchar*)malloc(sub_len + 1);
  if (sub == NULL)
    return NULL;

  memcpy(sub, str + start_byte, sub_len);
  sub[sub_len] = '\0';

  return sub;
}


guint
parseDays				(gchar*	inDayStr)
{
  gchar*	daysStr;
  gchar*	inDays;
  guint		outDays;
  guint		day1;
  guint		day2;
  gint		i;
  char		dNum;
  char		midChar;
  
  daysStr = trim(inDayStr);
  inDays = daysStr;
  outDays = 0x7F;
  day1 = 0;
  day2 = 0;
  if ((strcmp(daysStr,"") != 0) && (strlen(daysStr) > 1)) {
    //if a specific day of month, using format "x.dy"
    if ((strlen(daysStr) == 4) && (daysStr[1] == '.')) {
      day1 = day2Int(substr(daysStr,2,2));
      if (day1 > 0) {
        outDays = day1;
      }
    } else {
      //if multiple days using digits
      if (dayValid(daysStr, "nums")) {
        outDays = 0;
        //for dNum in inDays {
        for (i = 0; i < strlen(inDays); i++) {
          dNum = inDays[i];
          if ((dNum >= 49) && (dNum <= 55)) {
            outDays = outDays + (gint)(pow(2.0,(double)(dNum - 49)));
          }
        }
      } else {
        //if spelled-out days, either single, two (","), or range ("-")
        if (dayValid(daysStr, "lets")) {
          day1 = day2Int(substr(daysStr, 0, 2));
          if (day1 > 0) {
            if (strlen(daysStr) == 2) {
              outDays = day1;
            } else {
              if (strlen(daysStr) > 3) {
                day2 = day2Int(substr(daysStr, strlen(daysStr)-2, 2));
                if ((strlen(daysStr) == 4) && (day2 > 0)) {
                  outDays = day1+day2;
                } else {
                  if ((strlen(daysStr) == 5) && (day2 > 0)) {
                    midChar = daysStr[2];
                    if (midChar == ',') {
                      outDays = day1+day2;
                    } else {
                      if (midChar == '-') {
                        i = day1;
                        outDays = 0;
                        while (i != day2) {
                          outDays = outDays + i;
                          i = i * 2;
                          if (i > 64) {
                            i = 1;
                          }
                        }
                        outDays = outDays + day2;
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return outDays;
}


gboolean
daysMatch				(gchar*	inDayStr)
{
  return ((currentDow & parseDays(inDayStr)) > 0);
}


void
doInputTest				(GtkWidget*	sEntry)
{
  GtkEntryBuffer*	inBuff;
  const gchar*		inTest;
  gchar*		inTest2;
  gchar*		testText = "";
  double		test;

  inBuff = gtk_entry_get_buffer((GtkEntry*)sEntry);
  if (inBuff) {
    inTest = gtk_entry_buffer_get_text(inBuff);
    if (inTest) {
      inTest2 = strdup(inTest);
      testText = trim(inTest2);
      test = (atof)(testText);
      if (test != 0.0)
        get_freq_records(lookup_widget(tun1,"FreqList"),test);
      else
        get_stat_records(lookup_widget(tun1,"StatList"),testText,"","");
    }
  }
}

/* LATER - current lookup table may not include newly added languasges - creater from data and use code if no lookup exists.
void
doLanguageSearch()
{
  DispatchQueue.global(qos: .userInitiated).async { [weak self] in
    guard let self = self else {
      return
    }
    let testchar = "-"
    var test = ""
    if (self.readLines.count > 0) {
      for line in self.readLines {
        if (line.lengthOfBytes(using: String.Encoding.windowsCP1250) < 18) {
          test = ""
        } else {               
          test = String(line[line.index(line.startIndex, offsetBy: 18)..<line.index(line.startIndex, offsetBy: 19)])
        }
        if (test == testchar) {
          let language = String(line[line.index(line.startIndex, offsetBy: 59)..<line.index(line.startIndex, offsetBy: 62)]).trimmingCharacters(in: CharacterSet.whitespaces)
          let country = String(line[line.index(line.startIndex, offsetBy: 30)..<line.index(line.startIndex, offsetBy: 33)]).trimmingCharacters(in: CharacterSet.whitespaces)
          let target = String(line[line.index(line.startIndex, offsetBy: 63)..<line.index(line.startIndex, offsetBy: 66)]).trimmingCharacters(in: CharacterSet.whitespaces)
          var langTestData : String = self.getLookupOf(self.langLookup, key: "Code", value: language.trimmingCharacters(in: CharacterSet.whitespaces), lookupKey: "Language", defaultLookup: language)
          let countryTestData : String = self.getLookupOf(self.ITULookup, key: "Code", value: country.trimmingCharacters(in: CharacterSet.whitespaces), lookupKey: "Country", defaultLookup: country)
          let regionTestData : String = self.getLookupOf(self.regionLookup, key: "Code", value: country.trimmingCharacters(in: CharacterSet.whitespaces), lookupKey: "Region", defaultLookup: country)
          let targetTestData : String = self.getLookupOf(self.ITULookup, key: "Code", value: target.trimmingCharacters(in: CharacterSet.whitespaces), lookupKey: "Country", defaultLookup: target)
          if (langTestData != "") {
            let langTestChar : Character = langTestData.first!
            if (langTestData == language) && (langTestChar == ",") {
              var lang1Code : String = ""
              lang1Code.append(langTestData[langTestData.index(langTestData.startIndex, offsetBy: 0)])
              var lang2Code : String = ""
              lang2Code.append(langTestData[langTestData.index(langTestData.startIndex, offsetBy: 2)])
              let lang1 = self.getLookupOf(self.langLookup, key: "Code", value: lang1Code, lookupKey: "Language", defaultLookup: lang1Code)
              let lang2 = self.getLookupOf(self.langLookup, key: "Code", value: lang2Code as String, lookupKey: "Language", defaultLookup: lang2Code as String)
              langTestData = lang1+","+lang2
              self.langLookup.append(["Code":language,"Language":langTestData])
            }
          }
          if (self.langQueryData.contains(langTestData) != true) {
            self.langQueryData.append(langTestData)
          } //exists match
          if (self.countryQueryData.contains(countryTestData) != true) {
            self.countryQueryData.append(countryTestData)
          } //exists match
          if (self.regionQueryData.contains(regionTestData) != true) {
            self.regionQueryData.append(regionTestData)
          } //exists match
          if (self.targetQueryData.contains(targetTestData) != true) {
            self.targetQueryData.append(targetTestData)
          } //exists match
        }
      }
      self.langQueryData.sort()
      self.countryQueryData.append("")
      self.countryQueryData.sort()
      self.regionQueryData.sort()
      self.targetQueryData.sort()
    }
    DispatchQueue.main.async { [weak self] in
      self?.myLanguageList.reloadData()
    }
  }
}*/


struct response {
  char*		memory;
  size_t	size;
};

static size_t mem_cb			(void *contents,
                                         size_t size,
                                         size_t nmemb,
                                         void *userp)
{
  size_t realsize = size * nmemb;

  struct response *mem = (struct response *)userp;

  // Reallocate memory to accommodate the new data
  char* ptr = realloc(mem->memory, mem->size + realsize + 1);
  if (!ptr) {
    // Handle memory allocation error
    printf("not enough memory (realloc returned NULL)\n");
    return 0; // Return 0 to indicate an error
  }

  mem->memory = ptr;
  // Copy the received data into the buffer
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  // Add null termination
  mem->memory[mem->size] = 0;

  return realsize; // Return the number of bytes successfully handled
}


gboolean
checkWebPage				(gboolean	isBackground) 
{
  gchar*	dateLine = "";
  gchar*	fileLine = "";
  gchar*	dateStr = "";
  gchar*	fileStr = "";
  gchar*	fileURL;
  gchar*	fileURL2;
  gchar*	filePath;
  gchar*	filePath2;
  gchar*	fileExt;
  gchar*	test;
  time_t	newDate;
  gchar		msgStr[100];
  FILE*		fp;
  FILE*		fp2;
  CURL*		curl_handle;
  CURL*		curl_handle2;
  CURLcode	res;
  CURLcode	res2;
  gint		i;
  char		curDateStr[15];
  char		newDateStr[15];
  
  struct response
  chunk = {
    .memory = malloc(0),
    .size = 0
  };
  
  struct tm time_tm;

  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl_handle = curl_easy_init();
  if (curl_handle) {
    curl_easy_setopt(curl_handle, CURLOPT_URL, inPage);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, mem_cb);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
    res = curl_easy_perform(curl_handle);
    if (res != CURLE_OK) {
      g_print("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
      return false;
    }
    curl_easy_cleanup(curl_handle);
    curl_global_cleanup();
  }
  
  if (chunk.size > 0) {
    dateLine = strstr(chunk.memory, "Letzte &Auml;nderung:");
    dateLine = strstr(dateLine, "color:red;\">");
    dateLine = dateLine + 12;
    fileLine = strstr(chunk.memory, "dx/freq");
    fileStr = substr(fileLine, 0, 15);
    dateStr = substr(dateLine, 0, 11);
    if (dateStr[10] == '<') {
      for (i = 0; i < strlen(dateStr); i++)
        dateStr[10-i] = dateStr[9-i];
      dateStr[0] = '0';
    }
    test = strptime(dateStr, "%d %b %Y", &time_tm);
    time_tm.tm_hour = 0;
    time_tm.tm_min = 0;
    time_tm.tm_sec = 0;
#ifndef __MINGW64__
    time_tm.tm_gmtoff = 0;
    time_tm.tm_zone = "Z";
#endif
    time_tm.tm_isdst = 0;
    if (test != NULL) {
      newDate = mktime(&time_tm);
      if (newDate > db_date) {
        strftime(curDateStr, sizeof(curDateStr), "%d %b %Y", gmtime(&db_date));
        strftime(newDateStr, sizeof(newDateStr), "%d %b %Y", gmtime(&newDate));
        sprintf(msgStr, "You have: %s\nNew file: %s\n\nWould you like to download and use it now?", curDateStr, newDateStr);
        if (show_yes_no_message("An updated Eibi file is available", GTK_MESSAGE_QUESTION, msgStr) == GTK_RESPONSE_YES) {
          fileURL = g_strconcat(inPage, fileStr, NULL);
          fileURL2 = g_strconcat(inPage, "dx/README.TXT", NULL);
          fileExt = strrchr(fileStr,'.');
          fileExt[0] = '\0';
          newDateStr[2] = '-'; 
          filePath = g_strconcat(basepath, "/eibi-", strrchr(fileStr,'/')+1, "-", substr(newDateStr, 0, 6), ".txt", NULL);        
          filePath2 = g_strconcat(basepath, "/eibi-readme.txt", NULL);        
          curl_global_init(CURL_GLOBAL_DEFAULT);
          curl_handle = curl_easy_init();
          if (curl_handle) {
            fp = fopen(filePath, "wb");
            if (fp) {            
              curl_easy_setopt(curl_handle, CURLOPT_URL, fileURL);
              curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, NULL);
              curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, fp);
              res = curl_easy_perform(curl_handle);
              fclose(fp);
              if (res != CURLE_OK) {
                if (!isBackground)
                  show_message("Update Database",GTK_MESSAGE_ERROR,"Database not updated. Download failed.");
                g_print("Update database: Download failed: %s\n", curl_easy_strerror(res));
              } else {
                if (g_file_test(filePath, G_FILE_TEST_EXISTS)) {
                  //now update eibi-readme.txt
                  curl_handle2 = curl_easy_init();
                  if (curl_handle2) {
                    fp2 = fopen(filePath2, "wb");
                    if (fp2) {            
                      curl_easy_setopt(curl_handle2, CURLOPT_URL, fileURL2);
                      curl_easy_setopt(curl_handle2, CURLOPT_WRITEFUNCTION, NULL);
                      curl_easy_setopt(curl_handle2, CURLOPT_WRITEDATA, fp2);
                      res2 = curl_easy_perform(curl_handle2);
                      fclose(fp2);
                      if (res2 != CURLE_OK) {
                        if (!isBackground)
                          show_message("Update Database",GTK_MESSAGE_ERROR,"Database (README.TXT) not updated. Download failed.");
                        g_print("Update database: Download README.TXT failed: %s\n", curl_easy_strerror(res2));
                      } 
                    }
                    curl_easy_cleanup(curl_handle2);
                    g_free(filePath2);
                    g_free(fileURL2);
                  } 
                  db_name = filePath;
                  open_database();
                  if (!isBackground)
                    show_message("Update Database",GTK_MESSAGE_INFO,"Database has been successfully downloaded and updated.");
                }
              }
            } else {
              if (!isBackground)
                show_message("Update Database",GTK_MESSAGE_ERROR,"Database not updated. Error opening output file.");
              g_print("Update database: Error opening output file %s.\n", filePath);
            }
            curl_easy_cleanup(curl_handle);
            curl_global_cleanup();
            g_free(filePath);
            g_free(fileURL);
          } else {
            if (!isBackground)
              show_message("Update Database",GTK_MESSAGE_ERROR,"Database not updated. Error initializing libcurl.");
            g_print("Update database: Error initializing libcurl\n");
          }
        }
      }
      else {
        if (!isBackground)
          show_message("Update Database",GTK_MESSAGE_INFO,"You already have the latest database.");
        g_print("Update database: You already have the latest database.\n");
      }
    }
  } 
  free(chunk.memory);
  return true; 
}


void
open_eibi_file					(const char*	filename,
						 gchar**	contents)
{
  GIOChannel*	channel;
  GError*	err = NULL;
  FILE*		file;
  
  file = fopen(filename, "r");
  if (!file) {
    g_warning("Failed to open file: %s", g_strerror(errno));
    return;
  }
  channel = g_io_channel_unix_new(fileno(file));

  g_io_channel_set_encoding(channel, "WINDOWS-1250", &err);
  if (err) {
    g_warning("Failed to set encoding: %s", err->message);
    g_error_free(err);
    g_io_channel_unref(channel);
    fclose(file);
    return;
  }

  g_io_channel_read_to_end(channel, contents, NULL, &err);
  if (err) {
    g_warning("Failed to read file content: %s", err->message);
    g_error_free(err);
    g_io_channel_unref(channel);
    fclose(file);
    return;
  }

  g_io_channel_unref(channel);
  fclose(file);
}


void
getTextFileVerDate()
{
  gchar*	line;
  gchar*	sDate;
  gchar*	test;
  gint		i;
  gint		listlen;
  time_t	newDate;
  
  struct tm time_tm;
  
  listlen = g_slist_length(readLines);
  if (listlen > 0) {
    for (i = 0; i < listlen; i++) {
      line = (gchar*)g_slist_nth_data(readLines, i);
      if (line != NULL) {
        if (strlen(line) > 13) {
          if (strstr(line, "Last update: ") == line) {
            sDate = strdup(line + 13);
            test = strptime(sDate, "%b %d, %Y", &time_tm);
            time_tm.tm_hour = 0;
            time_tm.tm_min = 0;
            time_tm.tm_sec = 0;   
#ifndef __MINGW64__         
            time_tm.tm_gmtoff = 0;
            time_tm.tm_zone = "Z";
#endif
            time_tm.tm_isdst = 0;
            if (test != NULL) {
              newDate = mktime(&time_tm);
              if (newDate > db_date) {
                db_date = newDate;
                g_settings_set_int(sgc_settings,"last-db-date",db_date);
              }
            }
            g_free(sDate);
          }
        }
      }
    }
  }
}


void
open_database()
{
  gchar*		buf;
  gchar*		line;
  gchar*                eol;
  gchar*		file_path;
  gchar*		test;
  gchar*		test2;
  gchar*		exPtr;
  gchar*		lastCountry;
  gchar*		curCountry;
  gchar*		curCode;
  gchar*		curSite;
  gboolean		start;
  gint			i;
  gint			exPos;

  if (g_slist_length(readLines) > 0) {
    g_slist_free(readLines);
    readLines = NULL;
  }
  if (g_slist_length(readLines2) > 0) {
    g_slist_free(readLines2);
    readLines2 = NULL;
  }
  if (g_slist_length(XMTRCountries) > 0) {
    g_slist_free(XMTRCountries);
    XMTRCountries = NULL;
  }
  if (g_slist_length(XMTRCodes) > 0) {
    g_slist_free(XMTRCodes);
    XMTRCodes = NULL;
  }
  if (g_slist_length(XMTRSites) > 0) {
    g_slist_free(XMTRSites);
    XMTRSites = NULL;
  }

  num_freqs = 0;
  num_xmtrs = 0;
  start = false;
  
  //Open main file
  if (g_file_test(db_name, G_FILE_TEST_EXISTS)) {
    open_eibi_file (db_name, &buf);
    
    line = buf;
    while (line[0] != '\0') {
      eol = strchr(line, '\n');
      eol[0] = '\0';
      readLines = g_slist_append(readLines, strdup(line));
      line = eol+1;
    }
    num_freqs = g_slist_length(readLines);
    g_free(buf);

    //Open readme file
    file_path = get_pathname_from_full_path(db_name);
    db_name2 = g_strdup_printf("%s%s", file_path,"eibi-readme.txt");
    g_free(file_path);
    if (g_file_test(db_name2, G_FILE_TEST_EXISTS)) {
      open_eibi_file (db_name2, &buf);

      line = buf;
      while (line[0] != '\0') {
        eol = strchr(line, '\n');
        eol[0] = '\0';
    
        //Begin saving from first transmitter record
        if (!start) {
          if (strlen(line) > 8) {
            test = trim(substr(line, 3, 4));
            if (strcmp(test, "AFG:") == 0)
              start = true;
            g_free(test);
          }
        }

        if (start) {
          readLines2 = g_slist_append(readLines2, strdup(line));

          //End saving after last transmitter record
          if (strlen(line) > 8) {
            test = trim(substr(line, 3, 4));
            if (strcmp(test, "ZWE:") == 0)
              start = false;
            g_free(test);
          }
        }
        line = eol+1;        
      }
      num_xmtrs = g_slist_length(readLines2);
      g_free(buf);

      //Now parse into lookup table
      line = (gchar*)g_slist_nth_data(readLines2, 0);
      lastCountry = trim(substr(line, 3, 3));
      for (i=0; i < num_xmtrs; i++) {
        line = (gchar*)g_slist_nth_data(readLines2, i);
        test = trim(substr(line, 3, 3));
        test2 = trim(substr(line, 8, strlen(line)-8));

        if (strcmp(test, "") == 0)
          curCountry = strdup(lastCountry);
        else {
          if (test[strlen(test)-1] == ':')
            test[strlen(test)-1] = '\0';
          curCountry = strdup(test);
          g_free(lastCountry);
          lastCountry = strdup(curCountry);
        }
        g_free(test);

        //remove the ' except:' from some listings
        exPtr = strstr(test2," except:");
        if (exPtr != NULL) {
          exPos = exPtr - test2;
          test2[exPos] = '\0';
        }

        if (test2[1] == '-') {
          curCode = substr(test2,0,1);
          curSite = substr(test2,2,strlen(test2)-2);
        } else {
          if (test2[2] == '-') {
            curCode = substr(test2,0,2);
            curSite = substr(test2,3,strlen(test2)-3);
          } else {
            curCode = strdup("");
            curSite = strdup(test2);
          }
        }
        g_free(test2);

        XMTRCountries = g_slist_append(XMTRCountries, strdup(curCountry));
        XMTRCodes = g_slist_append(XMTRCodes, strdup(curCode));
        XMTRSites = g_slist_append(XMTRSites, strdup(curSite));
        g_free(curCountry);
        g_free(curCode);
        g_free(curSite);

      }
      //for (i=0; i < num_xmtrs; i++) {
      //  g_print("%3s: %2s-%s\n", (gchar*)g_slist_nth_data(XMTRCountries, i), (gchar*)g_slist_nth_data(XMTRCodes, i), (gchar*)g_slist_nth_data(XMTRSites, i));
      //}
      g_free(lastCountry);
      g_slist_free(readLines2);
      readLines2 = NULL;
    }
      
    g_settings_set_string(sgc_settings,"last-db",db_name);

    update_freq_db_display();
    if (get_visible(tun1, "TuningDlg"))
      doRefresh();

    getTextFileVerDate();
    db_ext_name = get_filename_from_full_path(db_name);
    set_app_title();
  }
}


void
read_statsType()
{
  gchar*	filename;
  GIOChannel*	channel;
  GError*	err = NULL;
  FILE*		file;
  gchar*	buf;
  gchar*	line;
  gchar*	eol;
  gint		typeInx;
  
  filename = g_strdup_printf("%s/%s",basepath,"statsTypeData.txt");

  if (g_file_test(filename, G_FILE_TEST_EXISTS)) {

    file = fopen(filename, "r");
    if (!file) {
      g_warning("Failed to open file: %s", g_strerror(errno));
      return;
    }
    channel = g_io_channel_unix_new(fileno(file));

    g_io_channel_read_to_end(channel, &buf, NULL, &err);
    if (err) {
      g_warning("Failed to read file content: %s", err->message);
      g_error_free(err);
      g_io_channel_unref(channel);
      fclose(file);
      return;
    }

    g_io_channel_unref(channel);
    fclose(file);

    line = buf;
    while (line[0] != '\0') {
      eol = strchr(line, ';');
      eol[0] = '\0';
      statsList = g_slist_append(statsList, strdup(line));
      line = eol+1;
      eol = strchr(line, '\n');
      eol[0] = '\0';
      if (strcmp(line, "Broadcast") == 0)
        typeInx = 0;
      else
        if (strcmp(line, "Broadcast Digital") == 0)
          typeInx = 1;
        else
          if (strcmp(line, "Utility Aircraft") == 0)
            typeInx = 2;
          else
            if (strcmp(line, "Utility Digital") == 0)
              typeInx = 3;
            else
              if (strcmp(line, "Utility Fixed/Land") == 0)
                typeInx = 4;
              else
                if (strcmp(line, "Utility Marine") == 0)
                  typeInx = 5;
                else
                  typeInx = 6; //Unknown
      typesList = g_slist_append(typesList, GINT_TO_POINTER(typeInx));
      line = eol+1;
    }
    g_free(buf);
    g_free(filename);
  }
}


gchar*
getCountryLookup				(gchar*	value,
                                                 gchar*	defaultLookup)
{
  gchar*	result = defaultLookup;
  gint		i;
  
  for (i = 0; i < ITUCount; i++) {
    if (strcmp(value, ITULookup[i].Code) == 0) {
      result = ITULookup[i].Country;
      break;
    }
  }
  return result;
}


gchar*
getCountryCode					(gchar*	value,
                                                 gchar*	defaultLookup)
{
  gchar*	result = defaultLookup;
  gint		i;
  
  for (i = 0; i < ITUCount; i++) {
    if (strcmp(value, ITULookup[i].Country) == 0) {
      result = ITULookup[i].Code;
      break;
    }
  }
  return result;
}


gchar*
getLanguageLookup				(gchar*	value,
                                                 gchar*	defaultLookup)
{
  gchar*	result = defaultLookup;
  gint		i;
  
  for (i = 0; i < langCount; i++) {
    if (strcmp(value, langLookup[i].Code) == 0) {
      result = langLookup[i].Language;
      break;
    }
  }
  return result;
}


gchar*
getLanguageCode					(gchar*	value,
                                                 gchar*	defaultLookup)
{
  gchar*	result = defaultLookup;
  gint		i;
  
  for (i = 0; i < langCount; i++) {
    if (strcmp(value, langLookup[i].Language) == 0) {
      result = langLookup[i].Code;
      break;
    }
  }
  return result;
}


gchar*
getTargetLookup					(gchar*	value,
                                                 gchar*	defaultLookup)
{
  gchar*	result = defaultLookup;
  gint		i;
  
  for (i = 0; i < ITUCount; i++) {
    if (strcmp(value, ITULookup[i].Code) == 0) {
      result = ITULookup[i].Country;
      break;
    }
  }
  return result;
}


gchar*
getTargetCode					(gchar*	value,
                                                 gchar*	defaultLookup)
{
  gchar*	result = defaultLookup;
  gint		i;
  
  for (i = 0; i < ITUCount; i++) {
    if (strcmp(value, ITULookup[i].Country) == 0) {
      result = ITULookup[i].Code;
      break;
    }
  }
  return result;
}


gchar*
getTransmitterLookup				(gchar*	c_value,
                                                 gchar*	t_value)
{
  gchar*	result = t_value;
  gchar*	temp_country;
  gchar*	temp_code;
  gint		dashPos = -1;
  gint		i;

  if (t_value[0] == '/') {
    if (strchr(t_value, '-') != NULL) {
      dashPos = strchr(t_value, '-') - t_value;
      temp_country = substr(t_value, 1, dashPos-1);
      temp_code = substr(t_value, dashPos+1, strlen(t_value)-dashPos-1);
    } else {
      temp_country = substr(t_value, 1, strlen(t_value)-1);
      temp_code = strdup("");
    }
  } else {
    temp_country = strdup(c_value);
    temp_code = strdup(t_value);
  }
  for (i=0; i < num_xmtrs; i++) {
    if (strcmp(temp_country, (gchar*)g_slist_nth_data(XMTRCountries, i)) == 0) {
      if (strcmp(temp_code, "") == 0) {
        result = (gchar*)g_slist_nth_data(XMTRSites, i);
        break;
      } else {
        if (strcmp(temp_code, (gchar*)g_slist_nth_data(XMTRCodes, i)) == 0) {
          result = (gchar*)g_slist_nth_data(XMTRSites, i);
          break;
        }
      }
    }
  }
  if (result != t_value) {
    if (strcmp(c_value, temp_country) != 0) {
      result = g_strdup_printf("[%s] %s",getCountryLookup(temp_country,temp_country),result);
    }
  }
  g_free(temp_country);
  g_free(temp_code);
  return result;
}

gint
getStatsType					(gchar*	value)
{
  gint		result = 6;
  gint		i;
  gint		listlen;
  
  listlen = g_slist_length(statsList);
  for (i = 0; i < listlen; i++) {
    if (strcmp(value, (gchar*)g_slist_nth_data(statsList,i)) == 0) {
      result = GPOINTER_TO_INT(g_slist_nth_data(typesList,i));
      break;
    }
  }
  return result;
}


double
convertTime				(gchar*	inTime)
{
  gchar*	hrStr;
  gchar*	minStr;
  gint		inHr;
  gint		inMin;
  double	inTim;

  hrStr = substr(inTime,0,2);
  inHr = atoi(hrStr);
  minStr = substr(inTime, 2,2);
  inMin = atoi(minStr);
  inTim = (double)inHr + (double)(inMin / 60.0);
  return inTim;
}


void
getAllRegionMatches				(gchar*		inRegion)
{
  gint		i;

  for (i = 0; i < regionCount; i++) {
    if (strcmp(regionLookup[i].Region, inRegion) == 0)
      regionList = g_slist_append(regionList, regionLookup[i].Code);
  }
}						 

gboolean
strContains					(gchar*	str1,
                                                 gchar*	str2,
                                                 gchar* str3)
{
  gchar*	temp;
  gchar*	pos;
  gboolean	result;
  
  temp = g_strconcat(str2, str3, NULL);
  pos = strstr(str1, temp);
  result = (pos != NULL);
  g_free(temp);
  
  return result;
}


gboolean
listContains					(GSList*	inList,
						 gchar*		inStr)
{
  gint		i;
  gchar*	temp;
  gboolean	result = false;
  
  if (g_slist_length(inList) > 0) {
    for (i = 0; i < g_slist_length(inList); i++) {
      temp = (gchar*)g_slist_nth_data(inList,i);
      result = (strcmp(inStr, temp) == 0);
      if (result)
        break;
    } 
  }
  return result;
}


gboolean
treestoreContains				(GtkTreeModel*	store,
                                                 gint		column,
                                                 gchar*		inStr,
                                                 GtkTreeIter	pIter[7])
{
  gint		i;
  gint		j;
  gint		listlen;
  GValue	value = G_VALUE_INIT;
  GtkTreeIter	iter;
  gboolean	result = false;
  
  if (store) {
    for (j = 0; j < 7; j++) {
      listlen = gtk_tree_model_iter_n_children((GtkTreeModel*)store, &pIter[j]);
      if (listlen > 0) {
        for (i = 0; i < listlen; i++) {
          gtk_tree_model_iter_nth_child((GtkTreeModel*)store, &iter, &pIter[j], i);
          gtk_tree_model_get_value((GtkTreeModel*)store, &iter, column, &value);
          result = (strcmp(inStr, g_value_get_string(&value)) == 0);
          g_value_unset(&value);
          if (result)
            break;
        }
      }
      if (result)
        break; 
    }
  }
  return result;
}		


gboolean
liststoreContains				(GtkTreeModel*	store,
                                                 gint		column,
                                                 gchar*		inStr)
{
  gint		i;
  gint		listlen;
  GValue	value = G_VALUE_INIT;
  GtkTreeIter	iter;
  gboolean	result = false;
  
  if (store) {
    listlen = gtk_tree_model_iter_n_children((GtkTreeModel*)store, NULL);
    if (listlen > 0) {
      for (i = 0; i < listlen; i++) {
        gtk_tree_model_iter_nth_child((GtkTreeModel*)store, &iter, NULL, i);
        gtk_tree_model_get_value((GtkTreeModel*)store, &iter, column, &value);
        result = (strcmp(inStr, g_value_get_string(&value)) == 0);
        g_value_unset(&value);
        if (result)
          break;
      }
    }
  }
  return result;
}		


void
getLanguageList					(GtkListStore*	store)
{
  gint		i;
  GtkTreeIter	iter;

  for (i = 0; i < langCount; i++) {
    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter,
                       0, langLookup[i].Language, -1);
  }
}


void
getCountryList					(GtkListStore*	store)
{
  gint		i;
  GtkTreeIter	iter;

  for (i = 0; i < ITUCount; i++) {
    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter,
                       0, ITULookup[i].Country, -1);
  }
}


void
getRegionList					(GtkListStore*	store)
{
  gint		i;
  GtkTreeIter	iter;

  for (i = 0; i < regionCount; i++) {
    if (!liststoreContains((GtkTreeModel*)store, 0, regionLookup[i].Region)) {
      gtk_list_store_append(store, &iter);
      gtk_list_store_set(store, &iter,
                         0, regionLookup[i].Region, -1);
    }
  }
}


gint
compare_strings					(GtkTreeModel*	model,
                                                 GtkTreeIter*	iter1,
                                                 GtkTreeIter*	iter2,
                                                 gpointer	user_data)
{
  gchar*	string1;
  gchar*	string2;
  gint		result;
  gint		col_id;
  
  if ((GTK_IS_TREE_STORE(model)) && 
     (gtk_tree_model_iter_has_child(model, iter1) || gtk_tree_model_iter_has_child(model, iter2)))
      result = 0; //don't sort level 1 nodes
  else {  
    if (GTK_IS_TREE_STORE(model))
      col_id = 1;
    else
      col_id = 0;  
    gtk_tree_model_get(model, iter1, col_id, &string1, -1);
    gtk_tree_model_get(model, iter2, col_id, &string2, -1);
    result = g_strcmp0(string1,string2);
    g_free(string1);
    g_free(string2);
  }  
  return result;
}


void
set_filter_menu_items				(gint		option_id)
{
  switch (option_id) {
    case 0 :
      set_checked(tun1, "unfiltered1", true);
      break;
    case 1 :
      set_checked(tun1, "languagefilt1", true);
      break;
    case 2 :
      set_checked(tun1, "countryfilt1", true);
      break;
    case 3 :
      set_checked(tun1, "regionfilt1", true);
      break;
    case 4 :
      set_checked(tun1, "targetfilt1", true);
      break;
  }
}


void
set_filter_data_box				(GtkWidget*	filterBox,
                                                 gint		option_id)
{
  GtkListStore*	store;
  GtkEntry*	entry;
  
  enum {
    TYPE = 0,
    NUM_COLUMNS
  };
  
  currentFilter = option_id;
  store = gtk_list_store_new(NUM_COLUMNS, G_TYPE_STRING, -1);
  entry = (GtkEntry*)gtk_bin_get_child((GtkBin*)filterBox);
  switch (currentFilter) {
    case 0 : //Unfiltered
      gtk_entry_set_text(entry, "");
      break;
    case 1 : //Language
      gtk_entry_set_text(entry, db_language_filt);
      getLanguageList(store);
      break;
    case 2 : //Country
      gtk_entry_set_text(entry, db_country_filt);
      getCountryList(store);
      break;
    case 3 : //Region
      gtk_entry_set_text(entry, db_region_filt);
      getRegionList(store);
      break;
    case 4 : //Target
      gtk_entry_set_text(entry, db_target_filt);
      getCountryList(store);
      break;
    default : 
      break;
  }
  db_filt = currentFilter;
  g_settings_set_int(sgc_settings,"last-filter", db_filt);
  gtk_tree_sortable_set_sort_column_id((GtkTreeSortable*)store,
                                   0, GTK_SORT_ASCENDING);
  gtk_tree_sortable_set_sort_func((GtkTreeSortable*)store,
                                   0, (GtkTreeIterCompareFunc)compare_strings, NULL, NULL);
  gtk_combo_box_set_model((GtkComboBox*)filterBox, (GtkTreeModel*)store);
  g_object_unref(store); 
}


void
prep_filter					(gchar*		inVal,
                                                 gchar**	outVal)
{
  if (*outVal)
    g_free(*outVal);
  *outVal = strdup(inVal);
}                                                 


void
get_filter_data_box				(GtkWidget*	filterBox,
                                                 gint		option_id)
{
  GtkEntry*	entry;
  gchar*	value;
  
  entry = (GtkEntry*)gtk_bin_get_child((GtkBin*)filterBox);
  value = (gchar*)gtk_entry_get_text(entry);
  switch (option_id) {
    case 0 : //Unfiltered
      get_stats_records(lookup_widget(tun1,"StationsList"), "", "", "", "", true);
      break;
    case 1 : //Language
      prep_filter(value,&db_language_filt);
      g_settings_set_string(sgc_settings,"last-language-filter",db_language_filt);
      get_stats_records(lookup_widget(tun1,"StationsList"), getLanguageCode(db_language_filt,""), "", "", "", true);
      break;
    case 2 : //Country
      prep_filter(value,&db_country_filt);
      g_settings_set_string(sgc_settings,"last-country-filter",db_country_filt);
      get_stats_records(lookup_widget(tun1,"StationsList"), "", getCountryCode(db_country_filt,""), "", "", true);
      break;
    case 3 : //Region
      prep_filter(value,&db_region_filt);
      g_settings_set_string(sgc_settings,"last-region-filter",db_region_filt);
      get_stats_records(lookup_widget(tun1,"StationsList"), "", "", db_region_filt, "", true);
      break;
    case 4 : //Target
      prep_filter(value,&db_target_filt);
      g_settings_set_string(sgc_settings,"last-target-filter",db_target_filt);
      get_stats_records(lookup_widget(tun1,"StationsList"), "", "", "", getCountryCode(db_target_filt,""), true);
      break;
    default : 
      break;
  }    
}


void
get_stats_records				(GtkWidget*	cList,
                                                 gchar*		inLanguage,
                                                 gchar*		inCountry,
                                                 gchar*		inRegion,
                                                 gchar*		inTarget,
                                                 gboolean	doExpand)
{
  GtkTreeStore*	statsQueryData;
  GtkTreeIter	iter;
  gchar*	line;
  char		testchar = '-';
  char		test = ' ';
  double	myTime;
  gchar*	testLanguageText;
  gchar*	testCountryText;
  gchar*	testRegionText;
  gchar*	testTargetText;
  gchar*	frequency;
  double	freq;
  gchar*	start;
  double	startTime;
  gchar*	stop;
  double	stopTime;
  gchar*	days;
  gchar*	country;
  gchar*	station;
  gchar*	language;
  gchar*	target;
  gint		i;
  GtkTreeIter	pIter[7];
  gint		newType;
  GdkPixbuf*	pixBuf[7];
  gchar*	pixNames[7] = {"BlueImg", "GreenImg", "YellowImg", "OrangeImg", "RedImg", "PurpleImg", "GrayImg"};	
  gchar*	treeCats[7] = {"Broadcast", "Broadcast Digital", "Utility Aircraft", "Utility Digital", "Utility Fixed/Land", "Utility Marine", "Unknown"};	
  
  enum {
    ICON = 0,
    STAT,
    WEIGHT,
    NUM_COLS
  };
  
  myTime = curTime;
  testLanguageText = trim(inLanguage);
  testCountryText = trim(inCountry);
  testRegionText = trim(inRegion);
  testTargetText = trim(inTarget);
  getAllRegionMatches(testRegionText);
  if (g_slist_length(readLines) > 0) {
    statsQueryData = gtk_tree_store_new( NUM_COLS, GDK_TYPE_PIXBUF, G_TYPE_STRING, G_TYPE_INT, -1);    
    for (i = 0; i < 7; i++) {
      pixBuf[i] = gtk_image_get_pixbuf((GtkImage*)lookup_widget(tun1,pixNames[i]));
      gtk_tree_store_append(statsQueryData, &pIter[i], NULL);
      gtk_tree_store_set(statsQueryData, &pIter[i],
                         ICON, pixBuf[i],
                         STAT, treeCats[i],
                         WEIGHT, PANGO_WEIGHT_BOLD,
                         -1);
    }
    for (i = 0; i < g_slist_length(readLines); i++) {
      line = (gchar*)g_slist_nth_data(readLines,i);
      if (strlen(line) < 18)
        test = ' ';
      else
        test = line[18];
      if (test == testchar) {
        frequency = trim((substr(line,0,7)));
        freq = atof(frequency);
        station = trim(substr(line, 34, 24));
        language = trim(substr(line, 59, 3));
        country = trim(substr(line, 30, 3));
        target = trim(substr(line, 63, 3));
        start = substr(line, 14, 4);
        startTime = convertTime(start);
        stop = substr(line, 19, 4);
        stopTime = convertTime(stop);
        days = substr(line, 24, 5);
        //if currentTime within range of record then add it to list
        if (((strcmp(testLanguageText,"") == 0) || ((strcmp(testLanguageText,"") != 0) &&
           ((strcmp(language,testLanguageText) == 0) || (strContains(language,testLanguageText,",")) || (strContains(language,",",testLanguageText)) ) )) &&
           ((strcmp(testCountryText,"") == 0) || ((strcmp(testCountryText,"") != 0) && ((strcmp(country,testCountryText) == 0)))) &&
           ((strcmp(testRegionText,"") == 0) || ((strcmp(testRegionText,"") != 0) && ((listContains(regionList,country))))) &&
           ((strcmp(testTargetText,"") == 0) || ((strcmp(testTargetText,"") != 0) && ((strcmp(target,testTargetText) == 0))))) {
          if ((((startTime < stopTime) && ((myTime >= startTime) && (myTime < stopTime))) ||
             ((startTime > stopTime) && ((myTime >= startTime) || (myTime < stopTime)))) && daysMatch(days)) {
            if (!(((freq >= 530.0) && (freq <= 1700.0)) || ((freq >= 153.0) && (freq <= 279.0))) || doIncludeBCB) {
              if (!treestoreContains((GtkTreeModel*)statsQueryData, STAT, station, pIter)) {
                 newType = getStatsType(station);
                 gtk_tree_store_append(statsQueryData, &iter, &pIter[newType]);
                 gtk_tree_store_set(statsQueryData, &iter,
                                    ICON, pixBuf[newType],
                                    STAT, station,
                                    WEIGHT, PANGO_WEIGHT_NORMAL,
                                    -1);
              }
            } //BCB match
          } //time match
        } //optional language match
      }
      
    } /* LATER FOR USER-ENTERED DATA 
    if (self.userDatabase.count > 0) {
      for userData in self.userDatabase {
        let frequency = userData.value(forKey: "Frequency") as! String
        let freq = Double(frequency)!
        let station = userData.value(forKey: "Station") as! String
        let language = userData.value(forKey: "Language") as! String
        let country = userData.value(forKey: "Country") as! String
        let target = userData.value(forKey: "Target") as! String
        let start = userData.value(forKey: "Start") as! String
        let startTime = self.convertTime(start)
        let stop = userData.value(forKey: "Stop") as! String
        let stopTime = self.convertTime(stop)
        let days = userData.value(forKey: "Days") as! String
        //if freq and currentTime within range of record then add it to list
        if (((testLanguageText == "") || ((testLanguageText != "") && ((language == testLanguageText) || (language.contains(testLanguageText+",")) || (language.contains(","+testLanguageText)) ) )) &&
           ((testCountryText == "") || ((testCountryText != "") && ((country == testCountryText)))) &&
           ((testRegionText == "") || ((testRegionText != "") && ((region.contains(country))))) &&
           ((testTargetText == "") || ((testTargetText != "") && ((target == testTargetText))))) {
          if (((startTime < stopTime) && ((curTime >= startTime) && (curTime < stopTime))) ||
             ((startTime > stopTime) && ((curTime >= startTime) || (curTime < stopTime)))) && self.daysMatch(inDayStr: days) {
            if !(((freq >= 530.0) && (freq <= 1700.0)) || ((freq >= 153.0) && (freq <= 279.0))) || (self.doIncludeBCB) {
              let newType : String = self.getLookupOf(self.statsTypeData, key: "Stations", value: station, lookupKey: "Type", defaultLookup: "Unknown")
              if (!self.stationTypes[self.stationTypesList.firstIndex(of:newType)!].stations.contains(station)) {
                self.stationTypes[self.stationTypesList.firstIndex(of:newType)!].stations.append(station)
              }
            } //BCB match
          } //time match
        } //optional language match
      }
    }*/
    
    g_slist_free(g_steal_pointer(&regionList));
    gtk_tree_sortable_set_sort_column_id((GtkTreeSortable*)statsQueryData,
                                    STAT, GTK_SORT_ASCENDING);
    gtk_tree_sortable_set_sort_func((GtkTreeSortable*)statsQueryData,
                                    STAT, (GtkTreeIterCompareFunc)compare_strings, NULL, NULL);
    gtk_tree_view_set_model((GtkTreeView*)cList, (GtkTreeModel*)statsQueryData);
    g_object_unref(statsQueryData);
    
    if (doExpand)
      gtk_tree_view_expand_all((GtkTreeView*)cList);
  }
}


void
get_freq_records				(GtkWidget*	cList,
                                                 gdouble	inFreq)
{
  GtkListStore*	freqQueryData;
  GtkTreeIter	iter;
  gchar*	line;
  char		testchar = '-';
  char		test = ' ';
  double	myTime;
  gint		i;
  gchar*	frequency;
  double	freq;
  gchar*	start;
  double	startTime;
  gchar*	stop;
  double	stopTime;
  gchar*	days;
  double	testFreq;
  gboolean	freqFilter;
  gchar*	country;
  gchar*	station;
  gchar*	language;
  gchar*	target;
  char		freqStr[10];
  gchar*	counStr;
  gchar*	langStr;
  gchar*	targStr;
  gchar*	xmtrStr;
  
  enum {
    FREQ = 0,
    FROM,
    TO,
    DAYS,
    LANG,
    TARG,
    COUN,
    STAT,
    XMTR,
    NUM_COLS
  };
  
  myTime = curTime;
  if (cList != lookup_widget(app1,"clist1"))
    freqFilter = get_down(tun1,"FreqTimeFilterBtn");
  else
    freqFilter = true;
  if (g_slist_length(readLines) > 0) {
    freqQueryData = gtk_list_store_new( NUM_COLS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
                                        G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, -1);
    for (i = 0; i < g_slist_length(readLines); i++) {
      line = (gchar*)g_slist_nth_data(readLines,i);
      if (strlen(line) < 18)
        test = ' ';
      else
        test = line[18];
      if (test == testchar) {
        frequency = trim((substr(line,0,7)));
        freq = atof(frequency);
        start = substr(line, 14, 4);
        startTime = convertTime(start);
        stop = substr(line, 19, 4);
        stopTime = convertTime(stop);
        days = substr(line, 24, 5);
        testFreq = inFreq;
        if ((testFreq != 0.0) &&
           ((freq >= (testFreq - 5.0)) && (freq <= (testFreq + 5.0)))) {
          if (!freqFilter || ((((startTime < stopTime) && ((myTime >= startTime) && (myTime < stopTime))) ||
             ((startTime > stopTime) && ((myTime >= startTime) || (myTime < stopTime)))) && daysMatch(days))) {
            country = trim(substr(line, 30, 3));
            station = trim(substr(line, 34, 24));
            language = trim(substr(line, 59, 3));
            target = trim(substr(line, 63, 3));
            sprintf(freqStr, "%7.1f", freq);
            langStr = getLanguageLookup(language, language);
            targStr = getTargetLookup(target, target);
            counStr = getCountryLookup(country, country);
            if (strlen(line) < 75)
              xmtrStr = strdup(" ");
            else {
              xmtrStr = getTransmitterLookup(country, trim(substr(line, 75, strlen(line)-75)));
            }
            gtk_list_store_append(freqQueryData, &iter);
            gtk_list_store_set(freqQueryData, &iter,
                               FREQ, freqStr,
                               FROM, start,
                               TO, stop,
                               DAYS, days,
                               LANG, langStr,
                               TARG, targStr,
                               COUN, counStr,              
                               STAT, station,
                               XMTR, xmtrStr,
                               -1);
          } //time match
        } //frequency match
      }
                
    } /* LATER FOR USER-ENTERED DATA 
    if (userDatabase.count > 0) {
      for userData in userDatabase {
        let frequency = userData.value(forKey: "Frequency") as! String
        let freq = Double(frequency)
        let start = userData.value(forKey: "Start") as! String
        let startTime = convertTime(start)
        let stop = userData.value(forKey: "Stop") as! String
        let stopTime = convertTime(stop)
        let days = userData.value(forKey: "Days") as! String
        let testFreq = Double(testFreqText)
        //if freq and currentTime within range of record then add it to list
        if (testFreq != nil) &&
           ((freq! >= (testFreq! - 5.0)) && (freq! <= (testFreq! + 5.0))) {
          if (!freqFilter || ((((startTime < stopTime) && ((myTime >= startTime) && (myTime < stopTime))) ||
             ((startTime > stopTime) && ((myTime >= startTime) || (myTime < stopTime)))) && daysMatch(inDayStr: days))) {
            let country = userData.value(forKey: "Country") as! String
            let station = userData.value(forKey: "Station") as! String
            let language = userData.value(forKey: "Language") as! String
            let target = userData.value(forKey: "Target") as! String
            freqQueryData.append([
              "Frequency": String(format: "%7.1f",freq!),
              "From": start,
              "To": stop,
              "Days": days,
              "Language": getLookupOf(langLookup, key: "Code", value: language.trimmingCharacters(in: CharacterSet.whitespaces), lookupKey: "Language", defaultLookup: language),
              "Target": getLookupOf(ITULookup, key: "Code", value: target.trimmingCharacters(in: CharacterSet.whitespaces), lookupKey: "Country", defaultLookup: target),
              "Country": getLookupOf(ITULookup, key: "Code", value: country.trimmingCharacters(in: CharacterSet.whitespaces), lookupKey: "Country", defaultLookup: country),
              "Station": station,
              "User" : true
            ])
          } //time match
        } //frequency match
      }
    } */
    
    currentFrequency = inFreq;    
    gtk_tree_view_set_model((GtkTreeView*)cList, (GtkTreeModel*)freqQueryData);
    g_object_unref(freqQueryData);
  }
} 


gchar*
str2lower					(gchar*	inStr)
{
  gint		i;
  gchar*	inStr2;
  
  inStr2 = strdup(inStr);  
  for (i = 0; inStr2[i] != '\0'; i++) {
    inStr2[i] = tolower(inStr2[i]);
  }
  return inStr2;
}


void
get_stat_records				(GtkWidget*	cList,
                                                 gchar*		inStation,
                                                 gchar*		inLanguage,
                                                 gchar*		inTarget)
{
  GtkListStore*	statQueryData;
  GtkTreeIter	iter;
  gchar*	line;
  char		testchar = '-';
  char		test = ' ';
  double	myTime;
  gchar*	testStationText;
  gchar*	testStationText2;
  gchar*	testLanguageText;
  gchar*	testTargetText;
  gint		i;
  gchar*	frequency;
  double	freq;
  gchar*	start;
  double	startTime;
  gchar*	stop;
  double	stopTime;
  gchar*	days;
  gboolean	statFilter;
  gchar*	country;
  gchar*	station;
  gchar*	station2;
  gchar*	language;
  gchar*	target;
  char		freqStr[10];
  gchar*	counStr;
  gchar*	langStr;
  gchar*	targStr;
  gchar*	xmtrStr;

  enum {
    FREQ = 0,
    FROM,
    TO,
    DAYS,
    LANG,
    TARG,
    COUN,
    STAT,
    XMTR,
    NUM_COLS
  };

  myTime = curTime;
  statFilter = get_down(tun1,"StatTimeFilterBtn");
  testStationText = trim(inStation);
  testLanguageText = trim(inLanguage);
  testTargetText = trim(inTarget);
  if (g_slist_length(readLines) > 0) {
    statQueryData = gtk_list_store_new( NUM_COLS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
                                        G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, -1);
    for (i = 0; i < g_slist_length(readLines); i++) {
      line = (gchar*)g_slist_nth_data(readLines,i);
      if (strlen(line) < 18)
        test = ' ';
      else
        test = line[18];
      if (test == testchar) {
        testStationText2 = str2lower(testStationText);
        station = trim(substr(line, 34, 24));
        station2 = str2lower(station);
        language = trim(substr(line, 59, 3));
        target = trim(substr(line, 63, 3));
        start = substr(line, 14, 4);
        startTime = convertTime(start);
        stop = substr(line, 19, 4);
        stopTime = convertTime(stop);
        days = substr(line, 24, 5);
        //if freq and currentTime within range of record then add it to list
        if ((strcmp(testStationText,"") != 0) && (strContains(station2,testStationText2,""))) {
          if (((strcmp(testLanguageText,"")  == 0) || ((strcmp(testLanguageText,"")  != 0) && 
             ((strcmp(language,testLanguageText) == 0) || (strContains(language,testLanguageText,",")) || (strContains(language,",",testLanguageText)) ) )) &&
             ((strcmp(testTargetText,"") == 0) || ((strcmp(testTargetText,"") != 0) && ((strcmp(target,testTargetText) == 0))))) {
            if (!statFilter || ((((startTime < stopTime) && ((myTime >= startTime) && (myTime < stopTime))) ||
               ((startTime > stopTime) && ((myTime >= startTime) || (myTime < stopTime)))) && daysMatch(days))) {
              frequency = trim((substr(line,0,7)));
              freq = atof(frequency);
              sprintf(freqStr, "%7.1f", freq);
              country = trim(substr(line, 30, 3));
              langStr = getLanguageLookup(language, language);
              targStr = getTargetLookup(target, target);
              counStr = getCountryLookup(country, country);
              if (strlen(line) < 75)
                xmtrStr = strdup(" ");
              else {
                xmtrStr = getTransmitterLookup(country, trim(substr(line, 75, strlen(line)-75)));
              }
              gtk_list_store_append(statQueryData, &iter);
              gtk_list_store_set(statQueryData, &iter,
                                 FREQ, freqStr,
                                 FROM, start,
                                 TO, stop,
                                 DAYS, days,
                                 LANG, langStr,
                                 TARG, targStr,
                                 COUN, counStr,              
                                 STAT, station,
                                 XMTR, xmtrStr,
                                 -1);
            } //time match
          } //optional language match
        } //station match
        g_free(station2);
        g_free(testStationText2);
      }
      
    } /* LATER FOR USER-ENTERED DATA
    if (userDatabase.count > 0) {
      for userData in userDatabase {
        let station = userData.value(forKey: "Station") as! String
        let language = userData.value(forKey: "Language") as! String
        let target = userData.value(forKey: "Target") as! String
        let start = userData.value(forKey: "Start") as! String
        let startTime = convertTime(start)
        let stop = userData.value(forKey: "Stop") as! String
        let stopTime = convertTime(stop)
        let days = userData.value(forKey: "Days") as! String
        //if freq and currentTime within range of record then add it to list
        if ((testStationText != "") && (station.lowercased().contains(testStationText.lowercased()))) {
          if (((testLanguageText == "") || ((testLanguageText != "") && ((language == testLanguageText) || (language.contains(testLanguageText+",")) || (language.contains(","+testLanguageText)) ) )) &&
             ((testTargetText == "") || ((testTargetText != "") && ((target == testTargetText))))) {
            if (!statFilter || ((((startTime < stopTime) && ((curTime >= startTime) && (curTime < stopTime))) ||
               ((startTime > stopTime) && ((curTime >= startTime) || (curTime < stopTime)))) && daysMatch(inDayStr: days))) {
              let frequency = userData.value(forKey: "Frequency") as! String
              let freq = Double(frequency)
              let country = userData.value(forKey: "Country") as! String
              statQueryData.append([
                "Frequency": String(format: "%7.1f",freq!),
                "From": start,
                "To": stop,
                "Days": days,
                "Language": getLookupOf(langLookup, key: "Code", value: language.trimmingCharacters(in: CharacterSet.whitespaces), lookupKey: "Language", defaultLookup: language),
                "Target": getLookupOf(ITULookup, key: "Code", value: target.trimmingCharacters(in: CharacterSet.whitespaces), lookupKey: "Country", defaultLookup: target),
                "Country": getLookupOf(ITULookup, key: "Code", value: country.trimmingCharacters(in: CharacterSet.whitespaces), lookupKey: "Country", defaultLookup: country),
                "Station": station,
                "User" : true
              ])
            } //time match
          } //optional language match
        } //station match
      }
    }*/

    currentStation = strdup(testStationText);
    currentStationLang = strdup(testLanguageText);
    currentStationTarget = strdup(testTargetText);
    gtk_tree_view_set_model((GtkTreeView*)cList, (GtkTreeModel*)statQueryData);
    g_object_unref(statQueryData);
  }
}


void
get_station_data					(GtkWidget*	selection,
                                                         gboolean	fromStations)
{
  GtkTreeModel*	model;
  GtkTreeIter	iter;
  GValue	value = G_VALUE_INIT;
  GValue	freqval = G_VALUE_INIT;
  gchar*	text;
  gchar*	freq;
  double	r_freq = 0.0;
  gboolean	fromFaceplate;
  
  if (selection) {
    fromFaceplate = (selection == lookup_widget(app1,"FaceplateSelection"));
    if (fromFaceplate)
      select_from_x_y(lookup_widget(app1,"clist1"), selection);
    if (gtk_tree_selection_get_selected((GtkTreeSelection*)selection, &model, &iter)) {
      if (!gtk_tree_model_iter_has_child(model, &iter)) {    
        if (fromStations)
          gtk_tree_model_get_value(model, &iter, 1, &value);
        else {
          gtk_tree_model_get_value(model, &iter, 7, &value);
          gtk_tree_model_get_value(model, &iter, 0, &freqval);
          freq = strdup(g_value_get_string(&freqval));
          r_freq = atof(freq);
          g_value_unset(&freqval);
          g_free(freq);
        }
        text = strdup(g_value_get_string(&value));
        if (text) {
          if ((db_filt == 1) && (!fromFaceplate))
            get_stat_records(lookup_widget(tun1,"StatList"), text, getLanguageCode(db_language_filt, ""), "");
          else
            if ((db_filt == 4) && (!fromFaceplate))
              get_stat_records(lookup_widget(tun1,"StatList"), text, "", getCountryCode(db_target_filt, ""));
            else
              get_stat_records(lookup_widget(tun1,"StatList"), text, "", "");
        }
        if (!fromFaceplate)
          tune_from_db(r_freq);
        g_value_unset(&value);
        g_free(text);
        if (!get_visible(tun1,"TuningDlg"))
          gtk_widget_set_visible(tun1, true);
      }
    }
  }
}


void
get_frequency_data					(GtkWidget*	selection)
{
  GtkTreeModel*	model;
  GtkTreeIter	iter;
  GValue	value = G_VALUE_INIT;
  gchar*	text;
  double	r_freq = 0.0;
  
  if (selection) {
    if (gtk_tree_selection_get_selected((GtkTreeSelection*)selection, &model, &iter)) {
      gtk_tree_model_get_value(model, &iter, 0, &value); 
      text = trim(strdup(g_value_get_string(&value)));
      r_freq = atof(text);
      if (text) {
        get_freq_records(lookup_widget(tun1,"FreqList"), r_freq);
        tune_from_db(r_freq);
      }
      g_value_unset(&value);
      g_free(text);
    }
  }
}


void
doRefresh()
{
  GtkWidget*	clist;
  
  update_freq_db_display();
  
  if (get_list_num_rows(tun1,"StationsList") > 0) {
    clist = lookup_widget(tun1,"StationsList");
    switch (currentFilter) {
      case 0 : //"Unfiltered" :
        get_stats_records(clist,"", "", "", "", true);
        break;
      case 1 : //"Language" :
        get_stats_records(clist, getLanguageCode(db_language_filt,""), "", "", "", true);
        break;
      case 2 : //"Country" :
        get_stats_records(clist, "", getCountryCode(db_country_filt,""), "", "", true);
        break;
      case 3 : //"Region" :
        get_stats_records(clist, "", "", db_region_filt, "", true);
        break;
      case 4 : //"Target" :
        get_stats_records(clist, "", "", "", getCountryCode(db_target_filt,""), true);
        break;
      default:
        break;
    }
  }
  
  refreshList("StatList");
  refreshList("FreqList");
}


void
refreshList				(gchar*	inList)
{
  GtkWidget*	clist;

  if (get_list_num_rows(tun1,inList) >= 0) {
    clist = lookup_widget(tun1,inList);
    if (strcmp(inList, "FreqList") == 0)
      get_freq_records(clist, currentFrequency);
    else
      if (strcmp(inList, "StatList") == 0)
        get_stat_records(clist, currentStation, currentStationLang, currentStationTarget); 
  }
}

