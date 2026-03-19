types = {
     "uint32"       : "UINT32"  ,
     "float"        : "FLOAT"   ,
     "std::string"  : "STRING"  ,
     "Math::Vec3"   : "VEC3"    ,
     "Math::Vec4"   : "VEC4"    ,
     "Math::Quat"   : "QUAT"    ,
     "AssetRef"     : "ASSET",
}

reverse_types = { v: k for k, v in types.items() }