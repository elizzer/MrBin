const mongoose = require("mongoose")

const schema = mongoose.Schema({
    Id:String,
    location:String,
    level:Number,
    smoke:Boolean,
    fillRate:Number,
    entryCount:{
        type:Number,
        default:0
    },
    collectedCount:Number
})

module.exports=mongoose.model("Data",schema)