
#define MY_DB_TABLE_SIZE 100000



void edb_printError(EDB_Status err)
{
    if(DEB_SD) Serial.print("ERROR: ");
    switch (err)
    {
        case EDB_OUT_OF_RANGE:
            if(DEB_SD) Serial.println("Recno out of range");
            break;
        case EDB_TABLE_FULL:
            if(DEB_SD) Serial.println("Table full");
            break;
        case EDB_OK:
        default:
            if(DEB_SD) Serial.println("OK");
            break;
    }
}


void init_sd_edb(){
 
  if(DEB_SD) Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    if(DEB_SD) Serial.println("initialization failed!");
    return;
  }
  if(DEB_SD) Serial.println("initialization done."); 

   // Check dir for db files
  if (!SD.exists("/PRESETS")) {
      if(DEB_SD) Serial.println("Dir for Db files does not exist, creating...");
      SD.mkdir("/PRESETS");
  }
     if (SD.exists(db_name)) {

        dbFile = SD.open(db_name, FILE_WRITE);

        // Sometimes it wont open at first attempt, espessialy after cold start
        // Let's try one more time
        if (!dbFile) {
            dbFile = SD.open(db_name, FILE_WRITE);
        }

        if (dbFile) {
            if(DEB_SD) Serial.print("Openning current table... ");
            EDB_Status result = db.open(0);
            if (result == EDB_OK) {
                if(DEB_SD) Serial.println("DONE");
            } else {
                if(DEB_SD) Serial.println("ERROR");
                if(DEB_SD) Serial.println("Did not find database in the file " + String(db_name));
                if(DEB_SD) Serial.print("Creating new table... ");
                db.create(0, MY_DB_TABLE_SIZE, (unsigned int)sizeof(mySettings));
                if(DEB_SD) Serial.println("DONE");
                return;
            }
        } else {
            if(DEB_SD) Serial.println("Could not open file " + String(db_name));
            return;
        }
    } else {
        if(DEB_SD) Serial.print("Creating table... ");
        // create table at with starting address 0
        dbFile = SD.open(db_name, FILE_WRITE);
        db.create(0, MY_DB_TABLE_SIZE, (unsigned int)sizeof(mySettings));
        if(DEB_SD) Serial.println("DONE");
    }
}



void edb_recordLimit()
{
    if(DEB_SD) Serial.print("Record Limit: ");
    if(DEB_SD) Serial.println(db.limit());
}


void edb_deleteOneRecord(int recno)
{
    if(DEB_SD) Serial.print("Deleting recno: ");
    if(DEB_SD) Serial.println(recno);
    db.deleteRec(recno);
}


void edb_deleteAll()
{
    if(DEB_SD) Serial.print("Truncating table... ");
    db.clear();
    if(DEB_SD) Serial.println("DONE");
}


void edb_countRecords()
{
    if(DEB_SD) Serial.print("Record Count: ");
    if(DEB_SD) Serial.println(db.count());
}

//
void edb_createRecords(int num_recs)
{
    if(DEB_SD) Serial.println("Creating Records... ");
    if(DEB_SD) Serial.println(sizeof(mySettings));
    for (int recno = 1; recno <= num_recs; recno++)
    {
        mySettings.preset = recno;
        if(DEB_SD) Serial.print("Nr. ");
        if(DEB_SD) Serial.print(recno);
        if(DEB_SD) Serial.print(", Name. ");
        if(DEB_SD) Serial.println(mySettings.presetName);
        //mySettings.presetName[6] = asciLookupTable[recno];
        //if(DEB_SD) Serial.println(mySettings.presetName);
        EDB_Status result = db.appendRec(EDB_REC mySettings);
        if (result != EDB_OK) edb_printError(result);
    }
    if(DEB_SD) Serial.println("DONE");
}


void edb_selectPreset(int recno)
{
    EDB_Status result = db.readRec(recno, EDB_REC mySettings);
    if (result == EDB_OK)
    {
        if(DEB_SD) Serial.print("PresetNr: ");
        if(DEB_SD) Serial.print(recno);
        if(DEB_SD) Serial.print(" Preset: ");
        if(DEB_SD) Serial.print(mySettings.preset);
        if(DEB_SD) Serial.print(" Name: ");
        if(DEB_SD) Serial.println(mySettings.presetName);
    }
    else edb_printError(result);
   
}


void edb_updateOneRecord(int recno)
{
    if(DEB_SD) Serial.print("Updating record at recno: ");
    if(DEB_SD) Serial.print(recno);
    if(DEB_SD) Serial.print("... ");
    //mySettings.preset = 1234;
    EDB_Status result = db.updateRec(recno, EDB_REC mySettings);
    if (result != EDB_OK) edb_printError(result);
    if(DEB_SD) Serial.println("DONE");
}


void edb_insertOneRecord(int recno)
{
    if(DEB_SD) Serial.print("Inserting record at recno: ");
    if(DEB_SD) Serial.print(recno);
    if(DEB_SD) Serial.print("... ");
    mySettings.preset = recno;
    mySettings.presetName[4] = random(50, 60);
    EDB_Status result = db.insertRec(recno, EDB_REC mySettings);
    if (result != EDB_OK) edb_printError(result);
    if(DEB_SD) Serial.println("DONE");
}


void edb_appendOneRecord(int id)
{
    if(DEB_SD) Serial.print("Appending record... ");
    mySettings.preset = id;
    mySettings.presetName[1] = random(1, 125);
    EDB_Status result = db.appendRec(EDB_REC mySettings);
    if (result != EDB_OK) edb_printError(result);
    if(DEB_SD) Serial.println("DONE");
}



