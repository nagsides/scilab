// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->
// ============================================================================
// Unitary tests for mxGetScalar mex function
// ============================================================================

cd(TMPDIR);
ilib_verbose(0);
mputl([ '#include ""mex.h""';
        '';
        'void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])';
        '{';
        '   double dblScalar = 0;';
        '   mxArray* pOut = NULL;';
        '';
        '   if(nrhs != 1 /*|| !mxIsDouble(prhs[0])*/)';
        '   {';
        '       mexErrMsgTxt(""Wrong number or type of input argument"");';
        '   }';
        '';
        '   dblScalar = mxGetScalar(prhs[0]);';
        '   pOut = mxCreateDoubleScalar(dblScalar);';
        '   plhs[0] = pOut;';
        '}'],'mexGetScalar.c');
ilib_mex_build('libmextest',['getScalar','mexGetScalar','cmex'], 'mexGetScalar.c',[],'Makelib','','','');
exec('loader.sce');

a = 1:10;
if getScalar(a) <> 1 then pause end
if getScalar(int8(a)) <> 1 then pause end
if getScalar(uint8(a)) <> 1 then pause end
if getScalar(int16(a)) <> 1 then pause end
if getScalar(uint16(a)) <> 1 then pause end
if getScalar(int32(a)) <> 1 then pause end
if getScalar(uint32(a)) <> 1 then pause end
if getScalar(int64(a)) <> 1 then pause end
if getScalar(uint64(a)) <> 1 then pause end
