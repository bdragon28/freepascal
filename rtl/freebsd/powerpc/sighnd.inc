{
   This file is part of the Free Pascal run time library.
   (c) 2000-2003 by Marco van de Voort
   member of the Free Pascal development team.

   See the file COPYING.FPC, included in this distribution,
   for details about the copyright.

   Signalhandler for FreeBSD/powerpc

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY;without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
}

procedure SignalToRunerror(Sig : longint; Info: PSigInfo; SigContext: PSigContext);public name '_FPC_DEFAULTSIGHANDLER';cdecl;
var
  res : word;
{  fpustate: longint; }
begin
  res:=0;
{$ifndef FPUNONE}
  { exception flags are turned off by kernel }
  fpc_enable_ppc_fpu_exceptions;
{$endif}
  case sig of
    SIGFPE :
          begin
            Case Info^.si_code Of
                FPE_INTDIV : Res:=200;  {integer divide fault. Div0?}
                FPE_FLTOVF : Res:=205;  {Overflow trap}
                FPE_FLTUND : Res:=206;  {Stack over/underflow}
                FPE_FLTRES : Res:=216;  {Device not available}
                FPE_FLTINV : Res:=216;  {Invalid floating point operation}
               Else
                Res:=208; {coprocessor error}
                End;
             sysResetFPU;
          End;
    SIGILL:
      res:=216;
    SIGBUS:
      res:=214;
    SIGSEGV :
      res:=216;
    SIGINT:
        res:=217;
    SIGQUIT:
        res:=233;
  end;
  {$ifdef FPC_USE_SIGPROCMASK}
   reenable_signal(sig);
  {$endif }
{ give runtime error at the position where the signal was raised }
  if res<>0 then
   begin
     HandleError(res);
   end;
end;
