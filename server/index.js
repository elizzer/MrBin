const express = require('express')
const cors= require('cors')
const bodyParser = require('body-parser')

const axios=require('axios')

const app= express()

const mongoose= require("mongoose")

const Data= require('./database')

app.use(cors())
app.use(bodyParser())


app.post('/sample',async(req,res)=>{
    console.log('[+]Req body ',req.body)
    let messageType=req.body.dataType
    let d=await Data.findOne({Id:req.body.ID})

    if(d===null){
        console.log('[+]Creating new bin...',d)
        d= new Data();
        d.Id=req.body.ID
        d.entryCount=0
        d.fillRate=0
        d.collectedCount=0
        d.level=0
        d.smoke=false
    }

    switch(messageType){
        case '0':
            if(req.body.level<0){
                console.log('[+]Unwanted data break...')
                break;
            }
            let interval=req.body.interval
            
            if(!(d.level<100 && d.level>0)){
                d.level=0
            }
            let fillRate= ((d.fillRate*d.entryCount)+(parseInt(req.body.level)-d.level))/(d.entryCount+1)
            console.log('[+}fill Rate',fillRate)
            // fillRate=((d.fillRate*d.entryCount)+((parseInt(req.body.level)-d.level)/parseInt(interval)))/(d.entryCount+1)
            d.fillRate=(fillRate===NaN)?0:fillRate
            
            d.level=req.body.level;
            d.entryCount=d.entryCount+1
            d.level=req.body.level
            console.log('[+]Level message updated log...',d)
            break;
        case '1':
            d.smoke=req.body.smoke
            break;
    }
    
    if(!(d.fillRate<100 && d.fillRate>0)){
        d.fillRate=0
    }

    console.log('[*]',d);
    d.save()

    res.status(200)
    res.send()
    return

})

app.get('/tocollect',async(req,res)=>{
    let time=12;
    let bins = await Data.find({level:{$gte:50}});
    bins= bins.map((bin)=>{
        if(bin.level<70){
            if((time*bin.fillRate+bin.level)>=90){
                return bin
            }
        }
        else{
            return bin;
        }
    })
    return res.json({
        bins:bins
    })
})

app.get('/action',async(req,res)=>{
    let bin = await Data.find({smoke:true})
    return res.json({
        bin:bin
    })
})

app.get('/createDummy',async(req,res)=>{
    id=10000;
    for(i=0;i<100;i++){
        lev=Math.random()*100
        console.log('[+]lev',lev)
        axios.post("http://localhost:4040/sample",{
            ID:id+i,
            dataType:'0',
            level:lev-(lev%10),
            interval:1
        
        })
    }

    return res.send()
})

app.post('/search',async(req,res)=>{
    let id = req.body.id;
    let bin;
    if(id!==""){
        bin= await Data.find({Id:id})
    }
    else{
        bin=await Data.find()
    }
    return res.json({
        bin:bin
    })
})

app.listen(4040,()=>{
    console.log('[+]Server online')
    mongoose.connect("mongodb://localhost:27017/esp").then(()=>{
        console.log('[+]Db connected')
    })
})