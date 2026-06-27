//
//  ThreadOperationQueue.mm
//  MicroMessenger
//
//  Created by yerungui on 12-12-18.
//

#import "comm/objc/ThreadOperationQueue.h"

#import "comm/objc/KFLogScopeAutoReleasePool.h"

static class __ThreadRunOnStart
{
public:
    __ThreadRunOnStart()
    {
        m_operationQueue = [[NSOperationQueue alloc] init];
        assert(m_operationQueue);
    }
    ~__ThreadRunOnStart()
    {
        m_operationQueue = nil;
    }
    
    NSOperationQueue* m_operationQueue;
} gs_threadrunonstart;

@interface __CThreadWarp : NSObject
{
    void (*m_funp)(void*);
    void* m_arg;
    void (*m_fun)();
    
}

-(void) Run;
@property (nonatomic, assign)  void (*m_funp)(void*);
@property (nonatomic, assign)  void (*m_fun)();
@property (nonatomic, assign)  void* m_arg;
@end

@implementation __CThreadWarp

@synthesize m_funp;
@synthesize m_fun;
@synthesize m_arg;

-(id) init
{
    if (self = [super init]) {
        m_funp = nil;
        m_arg = nil;
        m_fun = nil;
    }
    
    return self;
}

-(void) Run
{
    KFLOG_SCOPE_POOL();
    assert(nil!=m_funp || nil!=m_fun);
    if (nil!=m_funp)
        m_funp(m_arg);
    else
        m_fun();
        
}
@end


@interface __ThreadWarp : NSObject
{
    id m_target;
    SEL m_sel;
    id m_arg;
}

-(void) Run;
@property (nonatomic, strong)  id m_target;
@property (nonatomic, assign)  SEL m_sel;
@property (nonatomic, strong)  id m_arg;
@end

@implementation __ThreadWarp

@synthesize m_target;
@synthesize m_sel;
@synthesize m_arg;

-(id) init
{
    if (self = [super init]) {
        m_target = nil;
        m_sel = 0;
        m_arg = nil;
    }
    
    return self;
}

-(void) Run
{
    KFLOG_SCOPE_POOL();
    [m_target performSelector:m_sel withObject:m_arg];
}
@end

@implementation ThreadQueue

+(BOOL) RunWithTarget:(id)target selector:(SEL)sel object:(id)arg
{
    KFLOG_SCOPE_POOL();
    __ThreadWarp* warp = [[__ThreadWarp alloc] init];
    warp.m_target= target;
    warp.m_sel= sel;
    warp.m_arg = arg;
    
    NSInvocationOperation* invocation = [[NSInvocationOperation alloc]
                                         initWithTarget:warp selector:@selector(Run) object:nil];
    if (nil==invocation)
    {
        return NO;
    }

    [gs_threadrunonstart.m_operationQueue addOperation:invocation];
    return YES;
}

@end

extern "C" BOOL RunWithTarget(void (*_funp)(void*), void* _arg)
{
    KFLOG_SCOPE_POOL();
    __CThreadWarp* warp = [[__CThreadWarp alloc] init];
    warp.m_funp = _funp;
    warp.m_arg = _arg;

    NSInvocationOperation* invocation = [[NSInvocationOperation alloc]
                                         initWithTarget:warp selector:@selector(Run) object:nil];
    if (nil==invocation)
    {
        return NO;
    }

    [gs_threadrunonstart.m_operationQueue addOperation:invocation];
    return YES;
}

extern "C" BOOL RunWithTargetNoParam(void (*_fun)())
{
    KFLOG_SCOPE_POOL();
    __CThreadWarp* warp = [[__CThreadWarp alloc] init];
    warp.m_fun = _fun;

    NSInvocationOperation* invocation = [[NSInvocationOperation alloc]
                                         initWithTarget:warp selector:@selector(Run) object:nil];
    if (nil==invocation)
    {
        return NO;
    }

    [gs_threadrunonstart.m_operationQueue addOperation:invocation];
    return YES;
}

void comm_export_symbols_4(){}
