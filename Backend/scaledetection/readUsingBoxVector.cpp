struct box
{
    int left,top,bottom,right;
};

void readVecotorCoordi(vector<box> boxvec)
{
    int size = boxvec.size();
    PIX  *pixs;
    char *outText;
    const char *filename = "input1.png";
    const char *language = "eng";
    const char *datapath = "/usr/src/tesseract-ocr/";
    tesseract::PageSegMode pagesegmode = tesseract::PSM_SINGLE_COLUMN;  

    /*read*/
    if ( ( pixs = pixRead(filename) ) == NULL) 
    {
        printf("Unsupported image type of file %s.\n", filename);
        exit(3);
    }
 
    /* turn of dictionaries -> only possible during init*/
    GenericVector<STRING> vars_vec;
    vars_vec.push_back("load_system_dawg");
    vars_vec.push_back("load_freq_dawg");
    vars_vec.push_back("load_punc_dawg");
    vars_vec.push_back("load_number_dawg");
    vars_vec.push_back("load_unambig_dawg");
    vars_vec.push_back("load_bigram_dawg");
    vars_vec.push_back("load_fixed_length_dawgs");
 
    GenericVector<STRING> vars_values;
    vars_values.push_back("F");
    vars_values.push_back("F");
    vars_values.push_back("F");
    vars_values.push_back("F");
    vars_values.push_back("F");
    vars_values.push_back("F");
    vars_values.push_back("F");
 
    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
 
    api->Init(datapath, language, tesseract::OEM_DEFAULT,
              NULL, 0, &vars_vec, &vars_values, false);

    /* restrict to numbers*/
    /*api->SetVariable("tessedit_char_whitelist","0123456789.");
    api->SetVariable("language_model_penalty_non_dict_word", "0");*/
    api->SetImage(pixs);
    
    int extend = 0;
    for(int i = 0;i<size;i++)
    {
        api->SetRectangle(boxvec[i].left-extend,boxvec[i].top-extend,boxvec[i].right-boxvec[i].left+extend,boxvec[i].bottom-boxvec[i].top+extend);
        outText = api-> GetUTF8Text();
        cout<<"Text : "<<outText<<" "<<boxvec[i].left<<" "<<boxvec[i].top<<" "<<boxvec[i].right-boxvec[i].left<<" "<<boxvec[i].bottom-boxvec[i].top-extend<<"\n";
    }

   

    /*Finish*/
    api->End();
    delete [] outText;
    pixDestroy (&pixs);
} 