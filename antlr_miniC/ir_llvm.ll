; ModuleID = 'myModule'
source_filename = "myModule"

@a = common global [10 x i32] zeroinitializer
@i = common global i32 0
@0 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@1 = private unnamed_addr constant [3 x i8] c"%c\00", align 1
@2 = private unnamed_addr constant [3 x i8] c"%c\00", align 1

declare i32 @scanf(i8*, ...)

declare i32 @printf(i8*, ...)

define void @main() {
entry:
  store volatile i32 0, i32* @i, align 4
  br label %forCondBlock

forCondBlock:                                     ; preds = %forLoopBlock, %entry
  %load = load i32, i32* @i, align 4
  %bin_slt = icmp slt i32 %load, 12
  br i1 %bin_slt, label %forLoopBlock, label %forMergeBlock

forLoopBlock:                                     ; preds = %forCondBlock
  %load1 = load i32, i32* @i, align 4
  %gep = getelementptr [10 x i32], [10 x i32]* @a, i32 0, i32 %load1
  store volatile i32 1000, i32* %gep, align 4
  %load2 = load i32, i32* @i, align 4
  %funccall = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @0, i32 0, i32 0), i32 %load2)
  %funccall3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @1, i32 0, i32 0), i8 32)
  %load4 = load i32, i32* @i, align 4
  %bin_add = add i32 %load4, 1
  store volatile i32 %bin_add, i32* @i, align 4
  br label %forCondBlock

forMergeBlock:                                    ; preds = %forCondBlock
  %funccall5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @2, i32 0, i32 0), i8 10)
  ret void
}
