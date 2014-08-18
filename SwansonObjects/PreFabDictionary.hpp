/*
 * PreFabDictionary.hpp
 *
 *  Created on: Jul 17, 2014
 *      Author: Brandon
 */

#ifndef PREFABDICTIONARY_HPP_
#define PREFABDICTIONARY_HPP_

#include <string>
#include <set>
#include "Dictionary.hpp"
#include <iostream>
#include <cstdio>

using std::string;
using std::set;
using std::cout;

class PreFabDict{
private:

   //int numWords;

public:

   static Dictionary UsePreFabDict () {
       Dictionary myDict( PreFabDict::getSet() );

      if ( myDict.Filled() ) {
         cout << endl
               << "something strange has happened, we had an empty dictionary"
               << " but don't worry, we can still play with my limited stored vocabulary";
         cout << endl << "press anything to continue:";
         getchar();
         return myDict;
      } else {
         cout << endl
               << "something has gone very wrong here, we have no words, we cannot play";
         getchar();
         exit( 1 );
      }
   }

   static set<string> getSet(){

      int numWords=999;
      ///////////////////////////////////////////////////////////////////
      //////////////////////////////start copy///////////////////////////
      //////////////////////////////////////////////////////////////////
      string myWords[]= {  "nooky", "mutule", "thegn", "zoon", "tunes", "cartes", "apian", "palais", "seas", "blears", "fulham", "shies", "fuze", "flint", "taro",
 "maw", "bulges", "dialog", "torah", "darkie", "stoury", "kilts", "guacos", "chews", "enacts", "cloven", "tonier", "boy", "cats", "ascot",
 "feuar", "shroff", "sharer", "firm", "burg", "wings", "marker", "stale", "oxims", "summed", "acinic", "pish", "rook", "guid", "grimed",
 "gangly", "ship", "augurs", "masse", "baldly", "riped", "taler", "citify", "briefs", "north", "khakis", "author", "enroll", "jink", "sadism",
 "emesis", "shoat", "cabobs", "stairs", "gables", "leger", "plazas", "chivy", "hostly", "longan", "piqued", "yean", "banana", "pourer", "arsino",
 "mucins", "donate", "gloom", "teal", "weans", "unisex", "wanted", "mills", "satyr", "hooks", "awhile", "sabirs", "papain", "papers", "sync",
 "chufas", "gags", "feh", "haji", "liers", "korats", "glitch", "moped", "trike", "bree", "rigged", "babe", "honda", "phased", "fern",
 "moolas", "ripsaw", "duddy", "curia", "spinal", "bailie", "gob", "hylas", "bields", "makes", "viator", "out", "trod", "riping", "there",
 "frere", "haired", "gobans", "bootee", "elemi", "niched", "rays", "domain", "tenuto", "pawer", "birr", "nota", "dryest", "hemmed", "mayhem",
 "smalt", "repaid", "operas", "bigger", "drails", "posada", "pick", "sanger", "potash", "swives", "nixing", "cover", "lascar", "stolen", "semens",
 "pew", "awake", "looses", "fornix", "wheel", "penile", "dots", "place", "bowses", "hub", "skewer", "bag", "brashy", "inept", "their",
 "hits", "supine", "cohost", "saps", "ripply", "mucose", "cartop", "itself", "pearl", "merer", "terga", "chuffs", "lurid", "cayman", "biogas",
 "elbows", "coin", "parley", "fugal", "tapped", "spout", "sloyd", "call", "sweven", "galah", "calve", "lats", "lull", "dunked", "owes",
 "quacks", "rent", "wove", "hots", "oryxes", "toledo", "redos", "sowcar", "peep", "dismay", "baited", "sixty", "stalk", "tramel", "haps",
 "kopek", "spumed", "chomp", "kirns", "bows", "chicle", "dikey", "antis", "picot", "portal", "phones", "ort", "utile", "koines", "sullen",
 "pyxies", "renews", "rouser", "drat", "half", "genii", "mown", "lacked", "rumple", "senora", "tramel", "verst", "skiey", "banzai", "mutule",
 "moa", "sty", "ghaut", "sclaff", "hetero", "butane", "deans", "rayas", "hours", "pulis", "flinty", "kayos", "fumble", "soiled", "oleo",
 "mho", "thrawn", "zester", "datura", "legers", "phonon", "doty", "feds", "ghees", "forums", "coital", "tanist", "burier", "vomits", "momi",
 "record", "bite", "equal", "embeds", "ethos", "soups", "wimps", "clear", "curn", "tomcat", "dakoit", "peg", "nosey", "ranee", "senega",
 "jabber", "reink", "naric", "hooves", "tea", "veiner", "wonder", "sawn", "father", "egest", "busks", "scarph", "frays", "birse", "cats",
 "pastes", "jests", "strip", "kabobs", "rabic", "hissed", "jujube", "storax", "banish", "nurds", "meagre", "coz", "gauged", "spored", "pail",
 "yearn", "janes", "nutmeg", "ethers", "brute", "hales", "boobie", "chutes", "menus", "bran", "scheme", "doff", "magus", "melons", "sift",
 "viva", "siloed", "teuch", "masked", "corf", "quirk", "rets", "petite", "rites", "move", "handed", "viny", "riot", "paying", "snide",
 "comas", "jewed", "hunted", "ably", "sumacs", "proles", "sundew", "eterne", "carny", "vole", "pedlar", "tamp", "gates", "spaed", "filo",
 "seeder", "dug", "dabbed", "lefter", "iatric", "semes", "move", "nixies", "dourly", "biffy", "hallos", "coital", "years", "hijack", "quokka",
 "treaty", "chives", "whig", "dodo", "stager", "maills", "plum", "zooms", "buncos", "froes", "scram", "censor", "shalom", "gawk", "oomiak",
 "cavils", "decoct", "butyl", "dekkos", "iceman", "valuer", "serows", "afars", "cooees", "squat", "clanks", "gene", "images", "grat", "skeins",
 "tern", "urge", "cares", "lichen", "slimsy", "apeak", "wail", "rigors", "obese", "polite", "coiner", "padnag", "folium", "toast", "tall",
 "wifes", "edgers", "cower", "gybes", "lock", "linkup", "stumpy", "peavy", "mincer", "ginger", "tao", "visaed", "riper", "enacts", "puff",
 "hyp", "sneaky", "tends", "chorea", "jesuit", "wyte", "gambit", "infos", "lewder", "uvulas", "brawer", "yuk", "heft", "toros", "ache",
 "warp", "daven", "deets", "taler", "genip", "reseed", "selva", "rerigs", "drier", "torsks", "coked", "lariat", "gyro", "behoof", "heathy",
 "sacred", "rainy", "rend", "broomy", "inlets", "made", "stob", "quint", "herd", "leered", "sating", "oppugn", "dub", "galah", "opera",
 "faints", "fayed", "refed", "dumber", "jeans", "stow", "semens", "bagmen", "strick", "secpar", "probes", "selfs", "enacts", "cremes", "ovules",
 "acetic", "sudd", "skee", "kolas", "markka", "appels", "berry", "cestus", "carats", "milder", "fended", "latch", "thread", "emes", "gleds",
 "pizzas", "cadged", "area", "feast", "buhls", "cram", "nutsy", "zax", "leaded", "crump", "sudor", "skives", "limpsy", "mowed", "atonic",
 "shout", "wades", "combat", "spry", "poling", "butch", "dropt", "muntin", "spile", "sacked", "pilau", "filed", "amas", "clinal", "brit",
 "lousy", "equal", "indies", "tepal", "rodeos", "agha", "blur", "three", "diver", "legmen", "ruling", "stow", "currs", "vocal", "gross",
 "koodoo", "oath", "office", "slues", "puris", "rom", "nordic", "giro", "nudest", "decals", "minims", "tronas", "clonks", "raptor", "morgan",
 "capper", "log", "teem", "plew", "stitch", "swills", "rutile", "merge", "handed", "inanes", "seep", "aurist", "nerts", "gelds", "flab",
 "splint", "yourn", "godly", "mucors", "balder", "virtus", "unisex", "satay", "clues", "inspan", "litre", "hikes", "gaffes", "slayed", "gimps",
 "clevis", "dud", "chela", "cozie", "pimps", "girl", "gaster", "alphyl", "cloche", "natty", "halm", "lucre", "cyclic", "mogul", "ados",
 "triced", "height", "inhale", "octave", "skiing", "rocks", "branch", "sigils", "sonant", "bindi", "tomboy", "spaces", "stirs", "boreal", "bongs",
 "zeds", "weber", "redyed", "kaif", "client", "ag", "curd", "stinky", "sagged", "joys", "comic", "cony", "shends", "devoid", "filum",
 "soever", "samps", "flukey", "mog", "decoy", "casual", "insane", "lucks", "carl", "emetin", "ivies", "tondo", "frowst", "cubist", "reb",
 "abs", "kaphs", "lottes", "sall", "red", "ligure", "fritts", "feudal", "brant", "noncom", "dopers", "wrang", "blamer", "trijet", "jouncy",
 "flared", "plate", "piggin", "comate", "rins", "gussie", "sleigh", "census", "tear", "logics", "wikiup", "checks", "carboy", "filets", "stroy",
 "dazes", "owners", "looby", "acting", "spivs", "cantic", "amiga", "spader", "algal", "yuck", "sabre", "annul", "aide", "coops", "sheep",
 "baggy", "tweeze", "guppy", "snarl", "dears", "lease", "tints", "praos", "opines", "sinewy", "thirl", "okapis", "follow", "globby", "lotas",
 "rabat", "tussar", "azon", "drippy", "sneeze", "rainy", "beanie", "sides", "noshed", "manos", "ligure", "quippu", "neural", "bi", "fangas",
 "fledgy", "echos", "morgan", "optic", "emyde", "tussar", "spic", "couple", "line", "derry", "genres", "acute", "caller", "heifer", "basis",
 "nobly", "plats", "ganefs", "ibis", "new", "trifle", "spirit", "bilgy", "numb", "hutzpa", "abele", "juke", "trays", "crust", "legacy",
 "swivet", "yeomen", "wispy", "bonbon", "untied", "mocha", "gat", "goyish", "dearly", "accuse", "thrum", "mikvah", "jetty", "vowed", "gundog",
 "alamos", "pegged", "coacts", "mosk", "seggar", "swoons", "homer", "squeg", "snack", "rams", "puzzle", "funner", "batten", "folly", "cures",
 "lash", "crew", "star", "cloots", "heady", "unbox", "caws", "bawds", "robe", "dyvour", "turn", "lunted", "gemmed", "kanji", "reata",
 "tepal", "rests", "staphs", "vadose", "kibla", "coarse", "picric", "colead", "carpel", "wissed", "kreeps", "miring", "brads", "pilea", "molls",
 "legal", "elated", "whumps", "copeck", "biform", "tubae", "bicarb", "poesy", "wanner", "gulfs", "netted", "kirtle", "duct", "palet", "hyte",
 "wiener", "finder", "mother", "biddy", "tempo", "hented", "chine", "allay", "deform", "idem", "tsuris", "antres", "resee", "lumped", "scuff",
 "shawl", "alit", "cape", "outdo", "larks", "gruel", "habits", "powers", "airted", "cuke", "verbs", "lunula", "refire", "lum", "splay",
 "canons", "inlays", "keddah", "jogger", "lemony", "delude", "dongas", "limbs", "trolly", "boughs", "bemix", "bazars", "hexane", "active", "cony",
 "losing", "hussar", "sorned", "virion", "pas", "sorts", "glop", "incuse", "energy", "manics", "forced", "hagdon", "reuse", "etuis", "wadi",
 "woolen", "challa", "yird", "glioma", "mown", "jun", "riser", "bowyer", "careen", "raffle", "brewer", "slimsy", "buying", "waxily", "summit",
 "troves", "indigo", "bolo", "hakes", "allay", "bride", "abye", "lucres", "otiose", "lusty", "ragman", "octets", "hoax", "snaky", "simper",
 "jibes", "goalie", "hyssop", "rods", "eidola", "header", "gimps", "benign", "flayer", "arrear", "sials", "billed", "unrigs", "bonds", "kelims",
 "olein", "cushaw", "beery", "reaves", "fulled", "leman", "ephod", "limeys", "shirt", "poling", "wends", "indigo", "unreal", "roods", "urbia",
 "dorky", "creek", "clotty", "whop", "pink", "crabby", "bindi", "marrer", "redbay", "cookie", "numen", "aghast", "quoth", "giggly", "rezone",
 "shocks", "sins", "midst", "paused", "ascot", "peddle", "vinals", "robbed", "citole", "klongs", "sheals", "prows", "demark", "jete", "seemly",
 "rupees", "corker", "fife", "looies", "merit", "gumbo", "trams", "knacks", "gagaku", "euros",};
      ///////////////////////////////////////////////////////////////////
      ///////////////////////end copy////////////////////////////////////
      //////////////////////////////////////////////////////////////////

      set<string> words;
      for (int var = 0; var < numWords; ++var) {
         words.insert(myWords[var]);
      }

      return words;
   }
};

#endif /* PREFABDICTIONARY_HPP_ */
