#Über CVVisual
CVVisual ist eine Debug-Bibliothek für OpenCV, die verschiedene Möglichkeiten der Darstellung von Bildern und Ergebnissen von beispielsweise Filter- und Match-Operationen von OpenCV anbietet.  

##Benutzung: Beispiel
Ist die Bibliothek eingebunden, das CVVISUAL\_DEBUG-Makro definiert und die benötigten Header in den Code eingebunden, kann durch den Aufruf einer CVVisual-Funktion mit den von OpenCV gelieferten Daten als Argumenten das CVV-Hauptfenster geöffnet werden.

Beispielsweise könnte ein Codestück folgendermaßen aussehen:

	//...
	cvv::debugDMatch(src, keypoints1, src, keypoints2, match, CVVISUAL\_LOCATION);

![](../images_ueberblick/MainWindow.jpg)

Die Bilder werden zusammen mit Informationen und Metadaten in der Overview-Tabelle angezeigt.  
Ein Doppelklick darauf öffnet ein Tab, in dem die Bilder und Matches groß angezeigt werden.

![](../images_ueberblick/LineMatchViewTab.jpg)

In dieser Ansicht, genannt *Line Match View* werden die KeyPoints der Matches, d.h. die von OpenCV gelieferten ähnlichen Bildpunkte, durch Linien verbunden. Im Akkordeonmenü kann man beispielsweise deren Farbe änder. `Strg + Mausrad` erlaubt, zu zoomen.

![](../images_ueberblick/LineMatchViewZoomed.jpg)

Die Art der Darstellung kann im `View`-Dropdown-Menü geändert werden; so können die Matches etwa auch als Translationspfeile angezeigt werden.

![](../images_ueberblick/TranslationMatchViewTab.jpg)

Zudem gibt es bei Matches auch die Möglichkeit, die Daten in einer Tabelle anzuzeigen, im sogenannten 
*Raw View*. Die Daten können hier über einen Linksklick in einer JSON oder einer CSV-Datei gespeichert 
werden.

![](../images_ueberblick/RawviewTab.jpg)

Wird `Resume program execution` geklickt wird die Ausführung des zu debuggenden Programmes, das beim Aufruf des Hauptfensters angehalten wurde, fortgesetzt bis es auf eine weitere CVVisual-Funktion
stößt:

	//...
	cvv::debugFilter(src, dest, CVVISUAL\_LOCATION, filename);

Das Hauptfenster erscheint erneut, wobei der neuen Datensatz der Tabelle hinzugefügt wird.

![](../images_ueberblick/MainWindowTwoCalls.jpg)

Da es sich hier um eine Filter-Operation handelt, ist die Anzeige im Tab eine andere:

![](../images_ueberblick/DefaultFilterViewTab.jpg)

Auch die möglichen Anzeigen unterscheiden sich von denen für Match-Operationen.
Der *Dual Filter View* erlaubt zum Beispiel zusätzlich, ein Differenzbild der beiden übergebenen anzuzeigen.

![](../images_ueberblick/DualfilterViewDiffImg.jpg)

Nach einigen weiteren Schritten durch das Programm

	//...
	cvv::debugDMatch(src, keypoints1, src, keypoints2, match, CVVISUAL\_LOCATION)
	//...
	cvv::debugFilter(src, dest, CVVISUAL\_LOCATION, filename);
	//...
	cvv::debugFilter(src, dest, CVVISUAL\_LOCATION, filename); 
	//...
	cvv::debugDMatch(src, keypoints1, src, keypoints2, match, CVVISUAL\_LOCATION);
	//...
	cvv::showImage(img, CVVISUAL\_LOCATION);

ergibt sich im Overview folgendes Bild:

![](../images_ueberblick/MainWindowFull.jpg)

Dabei wird durch den letzten Aufruf nur ein einziges Bild zur Anzeige übergeben:

![](../images_ueberblick/SingleImageTab)

Mithilfe der Textzeile lassen sich durch Kommandos der *Filter Query Language* von CVVisual die Datensätze ordnen, filtern und gruppieren. Hier wurde nach ID gruppiert:

![](../images_ueberblick/OverviewFilterQueryGroupByID.jpg)

Dies funktioniert auch im *Raw View*.

Hinter dem letzten Aufruf einer regulären CVVisual-Funktion muss `finalShow` aufgerufen werden:

	//...
	cvv::finalShow();
	//...

Es wird ein weiteres Mal das Hauptfenster angezeigt; wird jedoch der nun `Exit program` benannte Knopf
betätigt, wird das Hauptfenster endgültig geschlossen.

![](../images_ueberblick/OverviewExitProgram.jpg)
  
Dies beschließt die Debug-Sitzung.

##Erweiterbarkeit: Beispiel
Ein Ziel der Entwicklung von CVVisual war es, die Bibliothek möglichst leicht erweiterbar zu halten.
In diesem Beispiel soll ein neuer *Filter View* - also eine Darstellungsmöglichkeit für einen Filter-Datensatz -
erstellt und eingefügt werden.

...


